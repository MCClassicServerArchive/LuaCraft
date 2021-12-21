#include "Various.h"
#include "md5.h"
#include "Player.h"

#include <list>

Player::Player(Server *srv, const char id_byte, SOCKET sockn) {
	this->bools_back = NULL;
	this->bools_front = NULL;
	this->strings_back = NULL;
	this->strings_front = NULL;
	this->numbers_back = NULL;
	this->numbers_front = NULL;
	this->CanDelete = false;
	this->isVisible = true;
	this->isHidden = false;
	this->isHidden = false;
	this->can_recv = false;
	this->server = srv;
	this->id = id_byte;
	this->sock = sockn;
	this->name = "";
	this->ip = "";
	this->op = false;
	this->pendingRelease = false;
	this->IsInMap = false;
	this->has_map = false;
	this->localListMutex = CreateMutex(NULL, false, NULL);
	this->packetQueueMutex = CreateMutex(NULL, false, NULL);
	//We will need some macros later, for *nix
}

Player::~Player() {
	// Call hook
	this->server->plugins->CallHook("OnPlayerLeave", this);
	while(!this->CanDelete) Sleep(1);
}

void Player::StartThread() {
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)playerThread, (void*)this, 0, NULL);
}

unsigned char Player::getId() {
	return id;
}

bool Player::getOp() {
	return op;
}

void Player::setOp(bool nop) {
	op = nop;
}

position Player::getPos() {
	return this->pos;
}

position Player::getNewPos() {
	return this->newpos;
}

SOCKET Player::getSock() {
	return sock;
}

void Player::closeSock() {
	closesocket(this->sock);
}

void Player::addLocalPlayer(class Player *player) {
	this->localPlayerList.push_back(player);
}

void Player::removeLocalPlayer(class Player *player) {
	unsigned char i;

	for(i = 0; i < this->localPlayerList.size(); ++i) {
		if(this->localPlayerList.at(i) == player) {
			this->sendDespawnPack(this->localPlayerList.at(i)->getId());
			this->localPlayerList.erase(this->localPlayerList.begin()+i);
		}
	}
}

void Player::handlePacket(const char *buffer) {
	switch(*buffer) { //first byte of *buffer
		case PACK_CODE_IN_HANDSHAKE:
			this->handleLoginPacket(buffer);
		break;

		case PACK_CODE_IN_BLOCK:
			if(this->IsInMap) this->handleBlockPacket(buffer);
		break;

		case PACK_CODE_IN_MOVEMENT:
			if(this->IsInMap) this->handleMovementPacket(buffer);
		break;

		case PACK_CODE_IN_CHAT:
			if(this->IsInMap) this->handleChatPacket(buffer);
		break;
	}
}

void Player::handleChatPacket(const char *buffer) {
	memset((char*)&this->lastChat[0], 0x00, 65);
	for(int i=0; i<64; i++) {
		if((buffer[i+2] == '\n') || (buffer[i+2] == '\r'))
			this->lastChat[i] = '\0';
		else
			this->lastChat[i] = buffer[i+2];
	}
	for(int i=63; i>=0; i--) {
		if(this->lastChat[i] != ' ') {
			this->lastChat[i+1] = '\0';
			break;
		}
	}
	this->server->plugins->CallHook("OnPlayerChat", this);
}

void Player::handleMovementPacket(const char *buffer) {
	memcpy(&this->newpos.x, buffer+2, PACK_SHORTLEN);
	memcpy(&this->newpos.y, buffer+4, PACK_SHORTLEN);
	memcpy(&this->newpos.z, buffer+6, PACK_SHORTLEN);
	
	this->newpos.x = this->server->getNetwork()->nntohs(this->newpos.x);
	this->newpos.y = this->server->getNetwork()->nntohs(this->newpos.y);
	this->newpos.z = this->server->getNetwork()->nntohs(this->newpos.z);
	
	this->newpos.heading = buffer[8];
	this->newpos.pitch = buffer[9];

	this->server->plugins->CallHook("OnPlayerMove", this);
}

void Player::handleBlockPacket(const char *buffer) {
	memcpy(&this->blockpos.x, buffer+1, PACK_SHORTLEN);
	memcpy(&this->blockpos.y, buffer+3, PACK_SHORTLEN);
	memcpy(&this->blockpos.z, buffer+5, PACK_SHORTLEN);
	
	this->blockpos.x = this->server->getNetwork()->nntohs(this->blockpos.x);
	this->blockpos.y = this->server->getNetwork()->nntohs(this->blockpos.y);
	this->blockpos.z = this->server->getNetwork()->nntohs(this->blockpos.z);

	this->blockpos.mode = buffer[7];
	this->blockpos.blocktype = buffer[8];
	if(this->level->validBlock(this->blockpos.x, this->blockpos.y, this->blockpos.z))
		this->blockpos.levelblock = this->level->getBlock(this->blockpos.x, this->blockpos.y, this->blockpos.z);
	else
		this->blockpos.levelblock = 128; //Invalid block

	this->server->plugins->CallHook("OnBlockChange", this);

}

char *Player::GetLastChat() {
	return (char*)&this->lastChat[0];
}

bool Player::validNonLoginPacket(const char PACK_CODE) {
	return (PACK_CODE != (const char)PACKET_BROKE_STREAM &&
			PACK_CODE != (const char)PACKET_ERROR &&
			PACK_CODE != (const char)PACK_CODE_IN_HANDSHAKE);
}

void Player::playerThread(void *params) {
	class Player *_this = (Player*)params;
	std::list<PendingPacket*>::iterator iter;
	const char *buffer;

	// Get initial packet
	buffer = (const char*) _this->getPacket();

	if(*buffer == PACK_CODE_IN_HANDSHAKE) {
		do {
			_this->handlePacket(buffer);
			_this->can_recv = true; 

			// Release mutex, free buffer, and give other threads a chance to snatch the mutex
			ReleaseMutex(_this->localListMutex);
			free((void*)buffer);
			Sleep(5);
			WaitForSingleObject(_this->localListMutex, INFINITE);

			// Send packets in queue
			_this->SendPackets();

			// Get new packet
			buffer = (const char*)_this->getPacket();
		} while(_this->validNonLoginPacket(*buffer));

		ReleaseMutex(_this->localListMutex);
		_this->can_recv = false;
		
		if(buffer[0] == PACK_CODE_IN_HANDSHAKE) { //Login should only be parsed once
			Console::PrintText("Duplicate login at %s", _this->name);
			_this->sendKick("You logged in from a different computer!");
		} else if(buffer[0] == PACKET_BROKE_STREAM) {
			Console::PrintText("Stream broken at %s", _this->name);
			_this->sendKick("Network Error!"); 
		} else {
			_this->server->releasePlayer(_this);
		}
	} else {
		_this->sendKick("Wrong first packet.");
	}

	free((void*)buffer);
	delete _this;
}

void Player::SendPackets() {
	// Loop through pending outgoing packets in the queue and send them
	WaitForSingleObject(this->packetQueueMutex, INFINITE);
	while(this->PacketQueue.size() > 0) {
		this->sendPacket(this->PacketQueue.front()->data, this->PacketQueue.front()->length);
		if(this->PacketQueue.front()->freeData)
			free((void*)this->PacketQueue.front()->data);
		delete this->PacketQueue.front();
		this->PacketQueue.erase(this->PacketQueue.begin());
	}
	ReleaseMutex(this->packetQueueMutex);
}

char *Player::getPacket() {

	unsigned short i;
	char *buffer;

	buffer = (char*)malloc(1);
	i = recv(this->sock, buffer, 1, 0);

	if( i < 1 ) {
		*buffer = PACKET_ERROR;
		return buffer;
	} else {
		if(*buffer == PACK_CODE_IN_HANDSHAKE)
			i = PACKLEN_IN_HANDSHAKE;
		else if(*buffer == PACK_CODE_IN_BLOCK)
			i = PACKLEN_IN_BLOCK;
		else if(*buffer == PACK_CODE_IN_MOVEMENT)
			i = PACKLEN_IN_MOVEMENT;
		else if(*buffer == PACK_CODE_IN_CHAT)
			i = PACKLEN_IN_CHAT;
		else {
			*buffer = PACKET_BROKE_STREAM;
			return buffer;
		}

		buffer = (char*)realloc(buffer, i);
		recv(this->sock, buffer+1, i-1, 0);
	}
	
	return buffer;
}

void Player::handleLoginPacket(const char *buffer) {
	if(buffer[1] != PROTO_VER) {
		sendKick("You are using the wrong Protocol!");
		return;
	}
	char hashbuf[64], hashed[33], *chr;
	unsigned char sig[16];

	buffer += 2;

	if((chr = strchr((char*)buffer, 0x20)) == NULL) {
		//error, deleting the player.
		server->releasePlayer(this);
		return;
	}

	*chr = 0;

	this->SetName(buffer);
	this->DisplayName = (char*)malloc(strlength(buffer)+1);
	strcpy(this->DisplayName, buffer);

	if(this->server->getVerifyNames()) {
		buffer += 64;
		chr = (char *)buffer+32;
		*chr = 0;

		sprintf(hashbuf, "%d%s", server->getSalt(), name); // salt, name
		md5_buffer(hashbuf, strlength(hashbuf), sig);
		md5_sig_to_string(sig, hashed, sizeof(hashed));

		if(strcmp(hashed, buffer) != 0) {
			sendKick("The server could not verify your name.");
			return;
		}
	}

	Player *tmpPlayer;

	tmpPlayer = server->GetPlayerByName(name);

	if((tmpPlayer != NULL) && (tmpPlayer != this)) {
		tmpPlayer->sendKick("You have logged in from another computer.");
	}

	this->sendWelcome();

	this->server->plugins->CallHook("OnPlayerJoin", this);
}

bool Player::sendWelcome() {
	unsigned char *packet;
	char srvName[64], srvMotd[64];

	packet = (unsigned char*)malloc(PACKLEN_OUT_SRVIDENTIFY);

	packet[0] = PACK_CODE_IN_HANDSHAKE;
	packet[1] = PROTO_VER;

	memset(srvName, 0x20, sizeof(srvName));
	memset(srvMotd, 0x20, sizeof(srvMotd));
	srvName[63] = 0;
	srvMotd[63] = 0;

	short nameLen = strlength(this->server->getServerName()+1);
	if(nameLen > sizeof(srvName)) nameLen = sizeof(srvName);

	short motdLen = strlength(this->server->getServerMotd()+1);
	if(motdLen > sizeof(srvMotd)) motdLen = sizeof(srvMotd);

	memcpy(srvName, this->server->getServerName(), nameLen);
	memcpy(srvMotd, this->server->getServerMotd(), motdLen);

	memcpy(packet+2, srvName, 64);
	memcpy(packet+2+64, srvMotd, 64);

	packet[2+64+64] = (this->GetOP() ? 0x64 : 0x00);

	this->sendPacket((const char*)packet, PACKLEN_OUT_SRVIDENTIFY);
	
	return true;
}

void Player::SendMessage(const char *message) {
	unsigned char *packet;
	char msg[64];

	packet = (unsigned char*)malloc(PACKLEN_IN_CHAT);

	packet[0] = PACK_CODE_IN_CHAT;
	packet[1] = PROTO_VER;

	memset(&msg, 0x20, 64);
	short msgLen = strlength(message);
	if(msgLen > 64) msgLen = 64;
	memcpy(&msg, message, msgLen);
	msg[63] = 0;

	memcpy(packet+2, &msg, 64);
	
	WaitForSingleObject(this->packetQueueMutex, INFINITE);
	this->PacketQueue.push_back(new PendingPacket(PACKLEN_IN_CHAT, (char*)packet, true));
	ReleaseMutex(this->packetQueueMutex);
}

bool Player::sendLevel(Level *lvl) {
	unsigned long sent = 0, left, total;
	unsigned short chunksize;
	unsigned char *mapSend, percentage, *mapBuf, *passMapBuf;

	mapBuf = (unsigned char*)malloc(PACKLEN_OUT_MAPCHUNK);

	mapSend = (unsigned char*)malloc(lvl->getCompressedMapLen() + 16384);
	total = gzip_compress(mapSend, lvl->getServerLevel_safe(), lvl->getDecompressedMapLen() - OUR_HEADERLEN);
	lvl->setCompressedMapLen(total);
	lvl->freeServerLevel_safe();

	send(this->sock, "\x02", 1, NULL);

	mapBuf[0] = PACK_CODE_OUT_MAPCHUNK;

	while(sent < total) {

		left = total - sent;
		chunksize = (left < 1024 ? (unsigned short) left : 1024);

		memcpy(mapBuf+3, mapSend+sent, chunksize);
		sent += chunksize;

		percentage = (unsigned char) ((sent * 100) / total);
		mapBuf[3+chunksize] = percentage;

		chunksize = this->server->getNetwork()->nhtons(chunksize);
		memcpy(mapBuf+1, &chunksize, sizeof(short));

		passMapBuf = (unsigned char*)malloc(PACKLEN_OUT_MAPCHUNK);
		memcpy(passMapBuf, mapBuf, PACKLEN_OUT_MAPCHUNK);
		
		this->sendPacket((const char*)passMapBuf, PACKLEN_OUT_MAPCHUNK);

		Sleep(2);
	}
	
	mapBuf[0] = (unsigned char)PACK_CODE_OUT_MAPFINISH;

	chunksize = lvl->getDimX();
	memcpy(mapBuf+1, &chunksize, sizeof(short));

	chunksize = lvl->getDimY();
	memcpy(mapBuf+3, &chunksize, sizeof(short));

	chunksize = lvl->getDimZ();
	memcpy(mapBuf+5, &chunksize, sizeof(short));

	passMapBuf = (unsigned char*)malloc(PACKLEN_OUT_MAPFINISH);
	memcpy(passMapBuf, mapBuf, PACKLEN_OUT_MAPFINISH);

	free(mapBuf);

	this->sendPacket((const char*)passMapBuf, PACKLEN_OUT_MAPFINISH);

	free(mapSend);

	position pos;

	pos.x = lvl->getSpawnX();
	pos.y = lvl->getSpawnY();
	pos.z = lvl->getSpawnZ();

	pos.heading = lvl->getRotX();
	pos.pitch = lvl->getRotY();
	
	this->pos = pos;
	this->savedpos = pos;

	this->Teleport(&pos);

	this->IsInMap = true;

	this->has_map = true;

	sendSpawns();

	return true;
}

bool Player::sendSpawnPack(unsigned char id_byte, char *name, position pos) {
	unsigned char *buffer;

	buffer = (unsigned char*)malloc(PACKLEN_OUT_SPAWN);
	memset(buffer, 0x20, PACKLEN_OUT_SPAWN);
	buffer[0] = PACK_CODE_OUT_SPAWN;

	if(id_byte == this->id)
		id_byte = -1;

	buffer[1] = id_byte;

	memcpy(buffer+2, name, strlength(name)+1);

	memcpy(buffer+66, &pos.x, 2);
	memcpy(buffer+68, &pos.y, 2);
	memcpy(buffer+70, &pos.z, 2);

	buffer[72] = pos.heading;
	buffer[73] = pos.pitch;

	WaitForSingleObject(this->packetQueueMutex, INFINITE);
	this->PacketQueue.push_back(new PendingPacket(PACKLEN_OUT_SPAWN, (char*)buffer, true));
	ReleaseMutex(this->packetQueueMutex);

	return true;
}

bool Player::sendSpawns() {

	std::vector<class Player*>::iterator iter;

	for(iter = localPlayerList.begin(); iter != localPlayerList.end(); ++iter) {
		//Display the character to everyone.
		if(strcmp((*iter)->GetWorld(), this->GetWorld()) == 0) {
			if(this->isHidden == false)
				(*iter)->sendSpawnPack(this->id, this->DisplayName, this->pos);
			if(((*iter)->id != this->id) && ((*iter)->isHidden == false))
				this->sendSpawnPack((*iter)->id, (*iter)->DisplayName, (*iter)->pos);
		}
	}
	this->sendSpawnPack(-1, this->DisplayName, this->pos);
	return true;
}

bool Player::SendBlockChange(block nblock) {
	unsigned char *packet;
	packet = (unsigned char*)malloc(PACKLEN_OUT_SENDBLOCK);
	unsigned short tmp;
	packet[0] = PACK_CODE_OUT_BLOCK;
	
	tmp = this->server->getNetwork()->nhtons(nblock.x);
	memcpy(packet+1, &tmp, PACK_SHORTLEN);

	tmp = this->server->getNetwork()->nhtons(nblock.y);
	memcpy(packet+3, &tmp, PACK_SHORTLEN);

	tmp = this->server->getNetwork()->nhtons(nblock.z);
	memcpy(packet+5, &tmp, PACK_SHORTLEN);


	if(nblock.blocktype > 0x31)
		nblock.blocktype -= 0x32;

	if(nblock.blocktype > 0x31)
		return false;

	packet[7] = nblock.blocktype;

	this->sendPacket((const char*)packet, PACKLEN_OUT_SENDBLOCK);
	return true;
}

bool Player::sendMovement(unsigned char id_byte, position oldpos, position newpos_other) {
	bool posnc = (oldpos.x == newpos_other.x && oldpos.y == newpos_other.y && oldpos.z == newpos_other.z);
	bool rotnc = (oldpos.heading == newpos_other.heading && oldpos.pitch == newpos_other.pitch); 
	
	if(false) {//Packet type 0x0B
		return this->sendMovementRot(id_byte, oldpos, newpos_other);
	} 
	else { // Packet Type 0x09
		return this->sendMovementHeadRot(id_byte, oldpos, newpos_other);
	}
	return false;
}

bool Player::sendMovementHeadRot(unsigned char id_byte, position oldpos, position newpos_other) {
	unsigned char *packet;
	packet = (unsigned char*)malloc(PACKLEN_OUT_MOVEANDHEAD);
	unsigned short tmp;
	packet[0] = PACK_CODE_OUT_TELEPORT;
	packet[1] = id_byte;
	
	tmp = this->server->getNetwork()->nhtons(newpos_other.x);
	memcpy(packet+2, &tmp, PACK_SHORTLEN);

	tmp = this->server->getNetwork()->nhtons(newpos_other.y);
	memcpy(packet+4, &tmp, PACK_SHORTLEN);

	tmp = this->server->getNetwork()->nhtons(newpos_other.z);
	memcpy(packet+6, &tmp, PACK_SHORTLEN);

	packet[8] = newpos_other.heading;
	packet[9] = newpos_other.pitch;

	WaitForSingleObject(this->packetQueueMutex, INFINITE);
	this->PacketQueue.push_back(new PendingPacket(PACKLEN_OUT_MOVEANDHEAD, (char*)packet, true));
	ReleaseMutex(this->packetQueueMutex);

	return true;
}

bool Player::sendMovementRot(unsigned char id_byte, position oldpos, position newpos_other) {
	unsigned char *packet;
	packet = (unsigned char*)malloc(PACKLEN_OUT_HEADING);

	packet[0] = PACK_CODE_OUT_HEADING;
	packet[1] = id_byte;

	packet[2] = newpos_other.heading;
	packet[3] = newpos_other.pitch;

	WaitForSingleObject(this->packetQueueMutex, INFINITE);
	this->PacketQueue.push_back(new PendingPacket(PACKLEN_OUT_HEADING, (char*)packet, true));
	ReleaseMutex(this->packetQueueMutex);

	return true;
}


bool Player::sendKick(const char *msg) {
	unsigned char *packet;
	packet = (unsigned char*)malloc(PACKLEN_OUT_DISCONNECT);
	
	packet[0] = (unsigned char)PACK_CODE_OUT_DISCONNECT;
	memset(packet+1, 0x20, 64);
	memcpy(packet+1, msg, strlength(msg));

	Console::PrintText("User [%s] kicked [%s]: \"%s\"", this->name, this->ip, msg);

	// Just send the kick packet from here - this case is special.
	send(this->sock, (const char*)packet, PACKLEN_OUT_DISCONNECT, NULL);

	this->server->releasePlayer(this);

	return true;
}

bool Player::sendDespawnPack(unsigned char id_byte) {
	unsigned char *packet;
	packet = (unsigned char*)malloc(PACKLEN_OUT_DESPAWN);

	packet[0] = PACK_CODE_OUT_DESPAWN;
	packet[1] = id_byte;

	WaitForSingleObject(this->packetQueueMutex, INFINITE);
	this->PacketQueue.push_back(new PendingPacket(PACKLEN_OUT_DESPAWN, (char*)packet, true));
	ReleaseMutex(this->packetQueueMutex);

	return true;
}

bool Player::sendPacket(const char *packet, int length) {
	return send(this->sock, packet, length, NULL);
}

// ==================================
//		LUA FUNCTIONS
// ==================================

char *Player::GetIP() {
	return this->ip;
}
char *Player::GetName() {
	return this->name;
}
char *Player::GetDisplayName() {
	return this->DisplayName;
}
void Player::SetName(const char *name) {
	this->name = (char*)malloc(strlength(name));
	strcpy(this->name, name);
}
void Player::SetDisplayName(const char *name) {
	try{
		free(this->DisplayName);
	} catch(...) {};
	this->DisplayName = (char*)malloc(strlength(name)+1);
	strcpy(this->DisplayName, name);
	this->server->DespawnPlayer(this);
	this->server->RespawnPlayer(this);
}
int Player::GetID() {
	return this->id;
}
bool Player::GetOP() {
	return this->op;
}
void Player::SetOP(bool OP) {
	this->op = OP;
}
void Player::Kick(const char *msg) {
	this->sendKick(msg);
}

bool Player::SetWorld(const char *worldName) {
	Level *tempLevel = this->server->GetLevelByName(worldName);
	if(!tempLevel)
		return false;
	this->levelStr = (char*)worldName;
	this->level = tempLevel;
	this->sendLevel(tempLevel);
	return true;
}
char *Player::GetWorld() {
	return this->levelStr;
}
bool Player::IsVisible() {
	return this->isVisible;
}
void Player::SetVisible(bool state) {
	if(!state) {
		this->server->DespawnPlayer(this);
		position tempPos = this->pos;
		this->pos.x = 0xFFFF;
		this->pos.y = 0xFFFF;
		this->pos.z = 0xFFFF;
		this->sendSpawns();
		this->pos = tempPos;
	}
	this->isVisible = state;
}
position *Player::GetPos() {
	return &this->pos;
}
position *Player::GetNewPos() {
	return &this->newpos;
}
short Player::GetX() {
	return this->pos.x;
}
short Player::GetY() {
	return this->pos.y;
}
short Player::GetZ() {
	return this->pos.z;
}
short Player::GetNewX() {
	return this->newpos.x;
}
short Player::GetNewY() {
	return this->newpos.y;
}
short Player::GetNewZ() {
	return this->newpos.z;
}
void Player::SetPos(position *posToSet) {
	this->pos = *posToSet;
	if(this->isVisible)
		this->server->sendMove(this);
}
void Player::SetX(short xToSet) {
	this->pos.x = xToSet;
	if(this->isVisible)
		this->server->sendMove(this);
}
void Player::SetY(short yToSet) {
	this->pos.y = yToSet;
	if(this->isVisible)
		this->server->sendMove(this);
}
void Player::SetZ(short zToSet) {
	this->pos.z = zToSet;
	if(this->isVisible)
		this->server->sendMove(this);
}
void Player::Teleport(position *targetPos) {
	this->pos = *targetPos;
	if(this->isVisible)
		this->server->sendMove(this);
	this->sendMovement(255, this->getPos(), *targetPos);
}
void Player::SendToPlayer(Player *targetPly) {
	this->Teleport(targetPly->GetPos());
}
void Player::Hide(bool state) {
	if(state) {
		this->savedpos = this->pos;
		this->server->DespawnPlayer(this);
	} else {
		this->pos = this->savedpos;
		this->server->RespawnPlayer(this);
		this->Teleport(&this->savedpos);	// Just in case.
	}
	this->isHidden = state;
}
void Player::SetBool(char *name, bool value) {
	PlayerBool *local = this->bools_front;
	while(local) {
		if(!strcmp(local->name, name)) {
			local->value = value;
			return;
		}
		local = local->next;
	}
	local = new PlayerBool();
	local->name = (char*)malloc(strlength(name));
	strcpy(local->name, name);
	local->value = value;
	if(this->bools_back)
		this->bools_back->next = local;
	if(!this->bools_front)
		this->bools_front = local;
	this->bools_back = local;
}
bool Player::GetBool(char *name) {
	PlayerBool *local = this->bools_front;
	while(local) {
		if(!strcmp(local->name, name))
			return local->value;
		local = local->next;
	}
	return NULL;
}
void Player::SetNumber(char *name, double value) {
	PlayerNumber *local = this->numbers_front;
	while(local) {
		if(!strcmp(local->name, name)) {
			local->value = value;
			return;
		}
		local = local->next;
	}
	local = new PlayerNumber();
	local->name = (char*)malloc(strlength(name)+1);
	strcpy(local->name, name);
	local->value = value;
	if(this->numbers_back)
		this->numbers_back->next = local;
	if(!this->numbers_front)
		this->numbers_front = local;
	this->numbers_back = local;
}
double Player::GetNumber(char *name) {
	PlayerNumber *local = this->numbers_front;
	while(local) {
		if(!strcmp(local->name, name))
			return local->value;
		local = local->next;
	}
	return NULL;
}
void Player::SetString(char *name, char *value) {
	PlayerString *local = this->strings_front;
	while(local) {
		if(!strcmp(local->name, name)) {
			local->value = (char*)realloc(local->value, strlength(value)+1);
			strcpy(local->value, value);
			return;
		}
		local = local->next;
	}
	local = new PlayerString();
	local->name = (char*)malloc(strlength(name)+1);
	strcpy(local->name, name);
	local->value = (char*)malloc(strlength(value)+1);
	strcpy(local->value, value);
	if(this->strings_back)
		this->strings_back->next = local;
	if(!this->strings_front)
		this->strings_front = local;
	this->strings_back = local;
}
char *Player::GetString(char *name) {
	PlayerString *local = this->strings_front;
	while(local) {
		if(!strcmp(local->name, name))
			return local->value;
		local = local->next;
	}
	return NULL;
}

block Player::GetLastBlock() {
	return this->blockpos;
}
