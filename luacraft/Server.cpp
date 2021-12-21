#include <fstream>

#include "Defines.h"
#include "Console.h"
#include "Network.h"
#include "Player.h"
#include "Structs.h"
#include "Server.h"
#include "Config.h"

Server::Server() {

	this->CanBeatFlist = false;
	time(&this->time_flist);
	time(&this->time_mcnet);

	Config cfg;
	char *rem_newline;

	Quit = false;

#ifdef _WIN32
	CreateDirectory((char*)"backups", NULL);
	CreateDirectory((char*)"plugins", NULL);
	CreateDirectory((char*)"gamemodes", NULL);
	CreateDirectory((char*)"data", NULL);
	CreateDirectory((char*)"cfg", NULL);
	CreateDirectory((char*)"maps", NULL);
#else
	mkdir("backups", NULL);
	mkdir("plugins", NULL);
	mkdir("gamemodes", NULL);
	mkdir("data", NULL);
	mkdir("cfg", NULL);
	mkdir("maps", NULL);
#endif

	// Load Config
	Console::DrawDivider("Config");

	this->server_name = (char*)malloc(65);
	this->server_name = (char*)cfg.getOption("name", true, "LuaCraft Custom Server", "Name of your server");
	Console::PrintText("Name is '%s'", this->server_name);

	this->max_players = atoi((char*)cfg.getOption("max-players", true, "16", "Maximum number of players online at once"));
	Console::PrintText("Max-players is %d", this->max_players);

	this->server_motd = (char*)malloc(65); 
	this->server_motd = (char*)cfg.getOption("motd", true, "LuaCraft Custom Server", "Message of the day");
	Console::PrintText("Motd is '%s'", this->server_motd);

	this->server_port = (char*)malloc(8); 
	this->server_port = (char*)cfg.getOption("port", true, "25565", "Port your server will use");
	Console::PrintText("Listening on port %s", this->server_port);

	this->server_public = cfg.asBool(cfg.getOption("public", true, "true", "Show your server in the public server list"));
	Console::PrintText("Server is %s", this->server_public ? "public" : "not public");

	this->server_verify_names = cfg.asBool(cfg.getOption("verify-names", true, "true", "Prevents name spoofing"));
	Console::PrintText("Verify-names is %s", this->server_verify_names ? "on" : "off");
	
	this->server_website = (char*)malloc(128);
	this->server_website = (char*)cfg.getOption("website", true, " ", "The servers website to display on fList");
	if(this->server_website)
		Console::PrintText("Server Website is %s", this->server_website);

	// Core Init
	Console::DrawDivider("Init");
	plugins = new PluginManager(this);
	plugins->InitLua();
	network = new Network(server_port);
	Console::PrintText("Initialized Networking");

	// Create space for hash
	this->server_hash = (char*)malloc(34);

	// Get hash and tinyurl
	this->BeatMinecraft();

	// Seed the pseudo-random number generator
	srand((unsigned int) time(0));
	salt = rand();
	this->Step();

	// Init Gamemode
	Console::DrawDivider("Gamemode");
	plugins->LoadGamemode((char*)cfg.getOption("game-mode", true, "Default", "The game-mode currently being used"));
	
	// Init Plugins system
	Console::DrawDivider("Plugins");
	plugins->LoadPlugins();

	// Print final divider
	Console::DrawDivider("Have fun!");

	// Call OnPluginsLoaded hook
	this->plugins->CallHook("OnPluginsLoaded", NULL);

	// Send heartbeats again
	this->BeatFlist();
	
	this->srvMutex = CreateMutex(NULL, false, NULL);
	this->mainListMutex = CreateMutex(NULL, false, NULL);
}

Server::~Server() {
	delete this->plugins;
	free(this->server_hash);
	free(this->server_name);
	free(this->server_motd);
	free(this->server_port);
}

Player* Server::getPlayerById(unsigned char id) {
	unsigned char i;

	for(i = 0; i < this->playerList.size(); ++i) {
		if(this->playerList.at(i)->getId() == id)
			return this->playerList.at(i);
	}

	return NULL;
}

Player* Server::GetPlayerByName(const char *name) {
	unsigned char i;

	for(i = 0; i < this->playerList.size(); ++i) {
		if(strcmp(this->playerList.at(i)->GetName(), name) == 0)
			return this->playerList.at(i);
	}

	return NULL;
}

unsigned char Server::getFreePlayerId() {
	unsigned char i;

	for(i = 0; i < this->playerList.size(); ++i) {
		if(this->getPlayerById(i) == NULL)
			break;
	}
	
	return i;
}

bool Server::releasePlayer(class Player *player) {
	unsigned char i, x;

	Player *player_main, *player_effected;
	
	/* I did some things here that might look odd, but I assure you,
		I did so with reason */


	try{
		/* Don't ask why, but access violation here under
			extreme conditions */
		if(this->playerList.size() < 1) return false;
	} catch(...) {
		try{
			player->Kill();
		} catch(...) {}
		return false;
	}
	WaitForSingleObject(this->mainListMutex, INFINITE);
	for(i = 0; i < this->playerList.size(); ++i) {
		if(this->playerList.at(i) == player) {
			player_main = this->playerList.at(i);
			if(player_main->pendingRelease)
				return false;
			player_main->pendingRelease = true;
			player_main->closeSock();
			
			for(x = 0; x < this->playerList.size(); ++x) {
				player_effected = this->playerList.at(x);
				if(WaitForSingleObject(player_effected->localListMutex, 1000) != WAIT_TIMEOUT) {
					player_effected->removeLocalPlayer(player_main);
					ReleaseMutex(player_effected->localListMutex);
				} else {
					ReleaseMutex(this->mainListMutex);
					player_effected->Kick("Error: Could not claim old player removal mutex");
					WaitForSingleObject(this->mainListMutex, INFINITE);
					this->playerList.at(x)->Kill();
				}
			}
			this->playerList.erase(this->playerList.begin()+i);
			ReleaseMutex(this->mainListMutex);
			return true;
		}
	}
	ReleaseMutex(this->mainListMutex);
	return false;
}

void Server::initPlayer(class Player *player, NetworkReturn *Ret) {
	player->ip = (char*)malloc(strlength(Ret->IP)*sizeof(char)+1);
	strcpy(player->ip, Ret->IP);
	player->StartThread();
}

bool Server::registerPlayer(class Player *player) {
	unsigned char i;
	this->playerList.push_back(player);

	if(WaitForSingleObject(this->playerList.back()->localListMutex, 1000) != WAIT_TIMEOUT) {
		for(i = 0; i < this->playerList.size(); ++i) {
			
			if(WaitForSingleObject(this->playerList.at(i)->localListMutex, 1000) != WAIT_TIMEOUT) {
				this->playerList.back()->addLocalPlayer(this->playerList.at(i));
				this->playerList.at(i)->addLocalPlayer(this->playerList.back());
				ReleaseMutex(this->playerList.at(i)->localListMutex);
			} else {
				this->playerList.at(i)->Kick("Error: Could not claim old player add mutex");
				this->playerList.at(i)->Kill();
			}

		}

		ReleaseMutex(this->playerList.back()->localListMutex);
	} else {
		this->playerList.back()->Kick("Error: Could not claim new player add mutex");
		this->playerList.back()->Kill();
	}

	return false;
}

void Server::DespawnPlayer(Player *player) {
	for(int i=0; i<this->playerList.size(); i++) {
		if(this->playerList.at(i) != player) {
			if(!this->playerList.at(i)->IsInMap)
				continue;
			this->playerList.at(i)->sendDespawnPack(player->getId());
		}
	}
}

void Server::RespawnPlayer(Player *player) {
	for(int i=0; i<this->playerList.size(); i++) {
		if(this->playerList.at(i) != player) {
			if(!this->playerList.at(i)->IsInMap)
				continue;
			if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
				this->playerList.at(i)->sendSpawnPack(player->GetID(), player->DisplayName, player->getPos());
		}
	}
}

void Server::SendMove(Player *player) {
	
	position delta;
	position newpos = player->getNewPos();
	position oldpos = player->getPos();
	bool posChanged, rotChanged;

	delta.x = newpos.x - oldpos.x;
	delta.y = newpos.y - oldpos.y;
	delta.z = newpos.z - oldpos.z;

	delta.heading = newpos.heading;
	delta.pitch = newpos.pitch;

	posChanged = delta.x != 0 || delta.y != 0 || delta.z != 0;
	rotChanged = newpos.heading != oldpos.heading || newpos.pitch != oldpos.pitch;

	// If the delta's are to big to fit in a byte.
	if( delta.x < -127 || delta.x > 127 || delta.y < -127 || delta.y > 127 || delta.z < -127 || delta.z > 127) {
		// Send everyone a teleport update since the deltas are too big.
		for(int i = 0; i < this->playerList.size(); ++i) {
			if(this->playerList.at(i) != player) {
				if(!this->playerList.at(i)->IsInMap)
					continue;
				if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
					this->playerList.at(i)->sendMovementTeleport(player->getId(), newpos);

			}
		}
	} else if(posChanged && rotChanged) {
			for(int i = 0; i < this->playerList.size(); ++i) {
			if(this->playerList.at(i) != player) {
				if(!this->playerList.at(i)->IsInMap)
					continue;
				if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
					this->playerList.at(i)->sendMovementHeadRot(player->getId(), delta);

			}
		}
	} else if(posChanged) {
		for(int i = 0; i < this->playerList.size(); ++i) {
			if(this->playerList.at(i) != player) {
				if(!this->playerList.at(i)->IsInMap)
					continue;
				if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
					this->playerList.at(i)->sendMovementHead(player->getId(), delta);

			}
		}
	} else if(rotChanged) {
		for(int i = 0; i < this->playerList.size(); ++i) {
			if(this->playerList.at(i) != player) {
				if(!this->playerList.at(i)->IsInMap)
					continue;
				if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
					this->playerList.at(i)->sendMovementRot(player->getId(), delta);

			}
		}

	} else {
		//Error? Just send a teleport then.
		for(int i = 0; i < this->playerList.size(); ++i) {
			if(this->playerList.at(i) != player) {
				if(!this->playerList.at(i)->IsInMap)
					continue;
				if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
					this->playerList.at(i)->sendMovementTeleport(player->getId(), newpos);

			}
		}
	}
}


int Server::getSalt() {
	return salt;
}

Network *Server::getNetwork() {
	return network;
}

char *Server::getServerMotd() {
	return this->server_motd;
}

char *Server::getServerName() {
	return this->server_name;
}

bool Server::getVerifyNames() {
	return this->server_verify_names;
}

bool Server::Step() {
	NetworkReturn *Ret;

	//Networking accept sock
	
	Ret = network->acceptConnections();

	if(Ret->tmpSock != (int)INVALID_SOCKET) {
		if(this->playerList.size() < this->max_players) {
			this->initPlayer(new Player(this, this->getFreePlayerId(), (SOCKET)Ret->tmpSock), Ret);
		} else {
			send(Ret->tmpSock, "\0x0EThe server is currently full.", 30, 0);
			closesocket(Ret->tmpSock);
		}

	}

	time(&timer);

	if(timer > time_mcnet) {
		this->BeatMinecraft();
		time_mcnet = timer + 45;
	}

	if((timer > time_flist) && this->CanBeatFlist) {
		this->BeatFlist();
		time_flist = timer + 90;
	}

	delete Ret;

	Sleep(100);

	return Quit;
}

void Server::BeatMinecraft() {
	static bool firstbeat = true;
	char *uriserver_name;

	char *tmpName;
	tmpName = (char*)malloc(strlength(this->server_name)+1);
	strcpy(tmpName, this->server_name);
	tmpName = stripColors(tmpName);
	uriserver_name = URIEncode(tmpName);

	if(firstbeat) {
		char *response, *tinyurl, *beatptr, *tmpptr;

		response = this->network->HttpPost("www.minecraft.net", "/heartbeat.jsp", true, "port=%s&users=%d&max=%d&name=%s&public=%s&version=7&salt=%l", this->server_port, this->playerList.size(), this->max_players, uriserver_name, this->server_public ? "true" : "false", this->salt);

		beatptr = strstr(response, "http://www.minecraft.net");
		char *tmpLineBr = strchr(beatptr, '\n');
		tmpLineBr[0] = '\0';

		if(beatptr != NULL) {
			FILE *exurl;
			exurl = fopen("external_url.txt", "w");
			fputs("Normal external URL:\n\n", exurl);
			fputs(beatptr, exurl);
			fputs("\n", exurl); // Add a breakline.

			tmpptr = URIEncode(beatptr); // Needed since URIEncode mallocs the string, so we will have to free it.
			tinyurl = this->network->HttpPost("tinyurl.com", "/api-create.php", true, "url=%s", tmpptr);
			free(tmpptr); 

			tmpptr = strstr(tinyurl, "http://");
			char *tmpLineBr = strchr(tmpptr, '\n');
			tmpLineBr[0] = '\0';

			fputs("\nTinyURL:\n\n", exurl);
			fputs(tmpptr, exurl);
			fclose(exurl);

			beatptr = strchr(beatptr, '=');
			strcpy(this->server_hash, beatptr+1);

			Console::PrintText("Ext. URL saved to file: external_url.txt");
			Console::PrintText("Also made a TinyURL: %s", tmpptr);
			firstbeat = false;

			free(tinyurl);
		}

		free(response);
	} else {
		this->network->HttpPost("www.minecraft.net", "/heartbeat.jsp", false, "port=%s&users=%d&max=%d&name=%s&public=%s&version=7&salt=%d", this->server_port, this->playerList.size(), this->max_players, uriserver_name, this->server_public ? "true" : "false", this->salt);
	}
	free(uriserver_name);
}

void Server::BeatFlist() {
	char *uriserver_name, *uriserver_motd, *playerlist, *worldlist, *website;
	int playerlistlen = 0;
	int worldlistlen = 0;
	bool displayWebsite = false;
	uriserver_name = URIEncode(this->server_name);
	uriserver_motd = URIEncode(this->server_motd);
	website = URIEncode(this->server_website);
	
	playerlist = (char*)malloc(1);
	playerlist[0] = 0x00;
	for(int i = 0; i < this->playerList.size(); i++) { // Build a comma seperated players list
		
		playerlist = (char*)realloc(playerlist, (strlength(this->playerList[i]->GetName())+4+playerlistlen));
			/* +4 instead of +3, for the null-terminator */
		strcpy(playerlist+playerlistlen, this->playerList[i]->GetName());
		playerlistlen += strlength(this->playerList[i]->GetName());
		
		if(i+1 < this->playerList.size()) {
			memcpy(playerlist+playerlistlen, "%2C", 3);
			playerlistlen += 3;
		}
	}

	bool FreePlyList = true;
	if(!playerlist[0]) {
		free(playerlist);
		playerlist = "%20";
		FreePlyList = false;
	}
	
	worldlist = (char*)malloc(1);
	worldlist[0] = 0x00;
	for(int i = 0; i < this->worldList.size(); i++) {
		
		worldlist = (char*)realloc(worldlist, (strlength(this->worldList[i]->Name)+4+worldlistlen));
			/* +4 instead of +3, for the null-terminator */
		strcpy(worldlist+worldlistlen, this->worldList[i]->Name);
		worldlistlen += strlength(this->worldList[i]->Name);
		
		if(i+1 < this->worldList.size()) {
			memcpy(worldlist+worldlistlen, "%2C", 3);
			worldlistlen += 3;
		}
	}

	bool FreeWorldList = true;
	if(!worldlist[0]) {
		free(worldlist);
		worldlist = "%20";
		FreeWorldList = false;
	}

	this->network->HttpPost("list.fragmer.net", "/announce.php", false, "name=%s&motd=%s&hash=%s&max=%d&users=%d&public=%s&players=%s&server=luacraft&worlds=%s&gamemode=%s&website=%s&data=%s", uriserver_name, uriserver_motd, this->server_hash, this->max_players, this->playerList.size(), this->server_public ? "true" : "false", playerlist, worldlist, this->plugins->GetGamemodeName(), website, DEFSERV_DATA); 
	
	//Bit of cleanup
	free(uriserver_name);
	free(uriserver_motd);
	free(website);

	try {
		if(FreePlyList)
			free(playerlist);
	} catch(...) {};

	this->CanBeatFlist = true;
}

// ===========================
// Lua functions for Server:
// ===========================

char *Server::GetGamemodeName() {
	return this->plugins->GetGamemodeName();
}

void Server::GetPluginByName(const char *pluginName) {
	// Don't use this now. Just use the statically-defined GetPluginByName()
	//return this->plugins->GetPluginByName(pluginName);
}

void Server::LoadPlugin(const char *fileName) {
	this->plugins->LoadPlugin((char*)fileName);
}void Server::PlaceBlock(block nblock, char* world) {
	for(int i = 0; i < this->playerList.size(); ++i) {
		if(!this->playerList.at(i)->IsInMap)
			continue;
		if(strcmp(this->playerList.at(i)->GetWorld(), world) == 0)
			this->playerList.at(i)->SendBlockChange(nblock);
	}

	this->GetLevelByName(world)->setBlock(nblock.x, nblock.y, nblock.z, nblock.blocktype);
}

block Server::GetBlock(char *world, short x, short y, short z) {
	block lblock;
	Level *wlevel;
	wlevel = this->GetLevelByName(world);
	
	lblock.blocktype = 128;
	lblock.x = x;
	lblock.y = y;
	lblock.z = z;

	if(wlevel == NULL) // Check if the level exist
		return lblock;
	
	if(!wlevel->validBlock(x, y, z)) // Make sure the block is valid
		return lblock;

	lblock.blocktype = wlevel->getBlock(x, y, z);

	return lblock;
}

std::vector<block> *Server::GetBlocksOfType(char *world, char type) {
	std::vector<block> retVec;
	Level *lvl;
	lvl = this->GetLevelByName(world);
	if(!lvl)
		return NULL;
	int dimX = lvl->getDimX();
	int dimY = lvl->getDimY();
	int dimZ = lvl->getDimZ();
	for(int x=0; x<dimX; ++x) {
		for(int y=0; y<dimY; ++y) {
			for(int z=0; z<dimZ; ++z) {
				block b = this->GetBlock(world, x, y, z);
				if(b.blocktype == type)
					retVec.push_back(b);
			}
		}
	}
	return &retVec;
}
	
int Server::GetWorldSizeX(char *world) {
	Level *lvl;
	lvl = this->GetLevelByName(world);
	if(lvl) {
		return lvl->getDimX();
	} else
		return NULL;
}

int Server::GetWorldSizeY(char *world) {
	Level *lvl;
	lvl = this->GetLevelByName(world);
	if(lvl) {
		return lvl->getDimY();
	} else
		return NULL;
}


int Server::GetWorldSizeZ(char *world) {
	Level *lvl;
	lvl = this->GetLevelByName(world);
	if(lvl) {
		return lvl->getDimZ();
	} else
		return NULL;
}	

void Server::CreateWorld(const char *worldName, const char *fileName) {
	Level *newLevel = new Level(fileName);
	newLevel->Name = (char*)malloc((strlength(worldName)+1));
	strcpy(newLevel->Name, worldName);
	this->worldList.push_back(newLevel);
	return;
}

void Server::SendChat(const char *message) {
	for(int i = 0; i < this->playerList.size(); ++i) {
		this->playerList.at(i)->SendMessage(message);
	}
}

void Server::SendChat(const char *message, const char *world) {
	for(int i = 0; i < this->playerList.size(); ++i) {
		if(strcmp(this->playerList.at(i)->GetWorld(), world) == 0)
			this->playerList.at(i)->SendMessage(message);
	}
}

char *Server::GetName() {
	return this->server_name;
}

char *Server::GetMotd() {
	return this->server_motd;
}

void Server::SetName(const char *name) {
	try {
		free(this->server_name);
	} catch(...) {};
	this->server_name = (char*)malloc(strlength(name)+1);
	strcpy(this->server_name, name);
}

void Server::SetMotd(const char *motd) {
	try {
		free(this->server_motd);
	} catch(...) {};
	this->server_motd = (char*)malloc(strlength(motd)+1);
	strcpy(this->server_motd, motd);
}

int Server::NumPlayers() {
	return this->playerList.size();
}

int Server::NumPlayersInWorld(const char *world) {
	int num = 0;
	for(int i=0; i<this->playerList.size(); ++i) if(!strcmp(this->playerList.at(i)->GetWorld(), world)) num++;
	return num;
}

int Server::MaxPlayers() {
	return this->max_players;
}

Level *Server::GetLevelByName(const char *worldName) {
	std::vector<Level*>::iterator iter;
	for(iter = worldList.begin(); iter != worldList.end(); ++iter) {
		if(strcmp((*iter)->Name, worldName) == 0) {
			return (*iter);
		}
	}
	return NULL;
}

void Server::DeleteWorld(const char *worldName) {
	std::vector<Level*>::iterator iter;
	for(iter = worldList.begin(); iter != worldList.end(); ++iter) {
		if(strcmp((*iter)->Name, worldName) == 0) {
			for(int i = 0; i < this->playerList.size(); ++i) {
				if(this->playerList.at(i)->level == (*iter))
					this->playerList.at(i)->Kick("ERROR: Your world no longer exists!");
			}
			delete (*iter);
			worldList.erase(iter);
			return;
		}
	}
}

std::vector<Player*> Server::GetPlayers() {
	return this->playerList;
}

std::vector<Player*> Server::GetPlayersInWorld(const char *world) {
	std::vector<Player*> retVec;
	for(int i=0; i<this->playerList.size(); ++i) if(!strcmp(this->playerList.at(i)->GetWorld(), world)) retVec.push_back(this->playerList.at(i));
	return retVec;
}

Zone *Server::CreateZone(char *_world, block _block1, block _block2) {
	Level *lvl = this->GetLevelByName(_world);
	if(!lvl)
		return NULL;
	return this->plugins->CreateZone(lvl, _block1, _block2);
}

bool Server::DeleteZone(Zone *_zone) {
	return this->plugins->RemoveZone(_zone);
}