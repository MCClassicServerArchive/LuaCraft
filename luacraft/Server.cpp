#include "Console.h"
#include "Network.h"
#include "Player.h"
#include "Structs.h"
#include "Server.h"
#include "Config.h"

Server::Server() {
	Config cfg;
	char config_output_buf[128], *rem_newline;

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

	this->server_name = (char*)cfg.getOption("name", true, "LuaCraft Custom Server", "Name of your server");
	Console::PrintText("Name is '%s'", this->server_name);

	this->max_players = atoi((char*)cfg.getOption("max-players", true, "16", "MAximum number of players online at once"));
	Console::PrintText("Max-players is %d", this->max_players);

	this->server_motd = (char*)cfg.getOption("motd", true, "LuaCraft Custom Server", "Message of the day");
	Console::PrintText("Motd is '%s'", this->server_motd);

	this->server_port = (char*)cfg.getOption("port", true, "25565", "Port your server will use");
	Console::PrintText("Listening on port %s", this->server_port);

	this->server_public = cfg.asBool(cfg.getOption("public", true, "true", "Show your server in the public server list"));
	Console::PrintText("Server is %s", this->server_public ? "public" : "not public");

	this->server_verify_names = cfg.asBool(cfg.getOption("verify-names", true, "true", "Prevents name spoofing"));
	Console::PrintText("Verify-names is %s", this->server_verify_names ? "on" : "off");
	
	// Core Init
	Console::DrawDivider("Init");
	plugins = new PluginManager(this);
	plugins->InitLua();
	network = new Network(server_port);
	Console::PrintText("Initialized Networking");

	// Init Gamemode
	Console::DrawDivider("Gamemode");
	plugins->LoadGamemode("Default");
	
	// Init Plugins system
	Console::DrawDivider("Plugins");
	plugins->LoadPlugins();

	// Print final divider
	Console::DrawDivider("Have fun!");

	// Call OnPluginsLoaded hook
	this->plugins->CallHook("OnPluginsLoaded", NULL);

	// Seed the pseudo-random number generator
	srand((unsigned int) time(0));
	salt = rand();
}

Server::~Server() {
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

	if(this->playerList.size() < 1) return false;
	for(i = 0; i < this->playerList.size(); ++i) {
		if(this->playerList.at(i) == player) {
			if(this->playerList.at(i)->pendingRelease)
				return false;
			this->playerList.at(i)->pendingRelease = true;
			this->playerList.at(i)->closeSock();
			
			for(x = 0; x < this->playerList.size(); ++x) {
				WaitForSingleObject(this->playerList.at(x)->localListMutex, INFINITE);
				this->playerList.at(x)->removeLocalPlayer(this->playerList.at(i));
				ReleaseMutex(this->playerList.at(x)->localListMutex);
			}
			this->playerList.erase(this->playerList.begin()+i);
			return true;
		}
	}

	Console::PrintText("Tried to clear non-existant player. Memory leak?");
	return false;
}

bool Server::addPlayer(class Player *player) {
	unsigned char i;
	this->playerList.push_back(player);

	WaitForSingleObject(this->playerList.back()->localListMutex, INFINITE);
	for(i = 0; i < this->playerList.size()-1; ++i) {
		WaitForSingleObject(this->playerList.at(i)->localListMutex, INFINITE);

		this->playerList.back()->addLocalPlayer(this->playerList.at(i));
		this->playerList.at(i)->addLocalPlayer(this->playerList.back());

		ReleaseMutex(this->playerList.at(i)->localListMutex);
	}
	ReleaseMutex(this->playerList.back()->localListMutex);

	player->StartThread();
	return false;
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
					this->playerList.at(i)->Kick("Your world no longer exists!");
			}
			delete (*iter);
			worldList.erase(iter);
			return;
		}
	}
}


void Server::DespawnPlayer(Player *player) {
	for(int i=0; i<this->playerList.size(); i++) {
		if(this->playerList.at(i) != player) {
			if(!this->playerList.at(i)->IsInMap)
				continue;
			if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
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
				this->playerList.at(i)->sendSpawnPack(player->GetID(), player->DisplayName, *player->GetPos());
		}
	}
}

void Server::sendMove(Player *player) {
	for(int i = 0; i < this->playerList.size(); ++i) {
		if(this->playerList.at(i) != player) {
			if(!this->playerList.at(i)->IsInMap)
				continue;
			if(strcmp(this->playerList.at(i)->GetWorld(), player->GetWorld()) == 0)
				this->playerList.at(i)->sendMovement(player->getId(), player->getPos(), player->getNewPos());

		}
	}
}

void Server::sendBlock(block nblock, char* world) {
	for(int i = 0; i < this->playerList.size(); ++i) {
		if(!this->playerList.at(i)->IsInMap)
			continue;
		if(strcmp(this->playerList.at(i)->GetWorld(), world) == 0)
			this->playerList.at(i)->SendBlockChange(nblock);
	}

	this->GetLevelByName(world)->setBlock(nblock.x, nblock.y, nblock.z, nblock.blocktype);
}

void Server::CreateWorld(const char *worldName, const char *fileName) {
	Level *newLevel = new Level(fileName);
	newLevel->Name = (char*)malloc(strlength(worldName)+1);
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
			this->addPlayer(new Player(this, this->getFreePlayerId(), (SOCKET)Ret->tmpSock));
			playerList.back()->ip = (char*)malloc(strlength(Ret->IP));
			strcpy(playerList.back()->ip, Ret->IP);
		} else {
			send(Ret->tmpSock, "\0x0EThe server is currently full.", 30, 0);
			closesocket(Ret->tmpSock);
		}

	}

	delete Ret;

	Sleep(100);

	return Quit;
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
}