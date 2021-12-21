#ifndef _PLAYER_H
#define _PLAYER_H
/*	============================================
		The Player class represents an client.
		It stores its variables, and had the
		code to interact with the client.
		Any networking code is in the Network
		class, so the Player doesn't handle
		networking directly.
	============================================ */

#include "Network.h"
#include "Level.h"
#include "Server.h"
#include "Structs.h"
#include "Level.h"

#ifndef _WIN32
	#include <pthread.h>
#endif

struct PendingPacket {
	int length;
	char *data;
	bool freeData;
	inline PendingPacket(int _length, char *_data, bool _freeData) {
		length = _length;
		data = _data;
		freeData = _freeData;
	}
};

struct PlayerBool {
	char *name;
	bool value;

	PlayerBool *next;
};

struct PlayerNumber {
	char *name;
	double value;

	PlayerNumber *next;
};

struct PlayerString {
	char *name;
	char *value;

	PlayerString *next;
};

class Player {
public:
	Player(Server *srv, const char id_byte, SOCKET sockn);
	~Player();

	void StartThread();

	bool CanDelete;
	bool getOp();
	bool IsInMap;
	void setOp(bool nop);
	unsigned char getId();
	SOCKET getSock();
	position getPos();
	position getNewPos();
	void closeSock();

	void passServer(Server *srv);

	void addLocalPlayer(class Player *player);
	void removeLocalPlayer(class Player *player);
#ifdef _WIN32
	HANDLE localListMutex;
#else
	pthread_mutex_t localListMutex;
#endif

	Level *level;
	char *levelStr;

	std::vector<PendingPacket*> PacketQueue;
#ifdef _WIN32
	HANDLE packetQueueMutex;
#else
	pthread_mutex_t packetQueueMutex;
#endif

	char *DisplayName;

	// LUA FUNCTIONS
	// I know there are some duplicates.
	// Do not use these functions with C++. These are reserved for the plugins only.
	// No Touchie! (Unless you know what you're doing)
	char *GetIP();

	char *GetName();
	void SetName(const char *name);
	char *GetDisplayName();
	void SetDisplayName(const char *name);

	int GetID();

	bool GetOP();
	void SetOP(bool OP);

	void Kill();
	void Kick(const char *msg);
	bool pendingRelease;

	bool SetWorld(const char *worldName);
	char *GetWorld();

	char *GetLastChat();
	void SendMessage(const char *message);

	bool SendBlockChange(block nblock);
	block GetLastBlock();

	void SetVisible(bool state);
	bool IsVisible();

	Position GetPos();
	Position GetNewPos();
	double GetX();
	double GetY();
	double GetZ();
	double GetNewX();
	double GetNewY();
	double GetNewZ();

	void SetPos(Position *posToSet);
	void SetX(double xToSet);
	void SetY(double yToSet);
	void SetZ(double zToSet);

	void Teleport(Position *targetPos);
	void SendToPlayer(Player *targetPly);
	void Hide(bool state);

	char *ip;

	void SetBool(char *name, bool value);
	bool GetBool(char *name);

	void SetNumber(char *name, double value);
	double GetNumber(char *name);

	void SetString(char *name, char *value);
	char *GetString(char *name);

	bool sendSpawnPack(unsigned char id_byte, char *name, position pos);
	bool sendDespawnPack(unsigned char id_byte);
	bool sendMovementHeadRot(unsigned char id_byte, position delta);
	bool sendMovementRot(unsigned char id_byte, position delta);
	bool sendMovementHead(unsigned char id_byte, position delta);
	bool sendMovementTeleport(unsigned char id_byte, position newpos_other);

	void SetMOTD(const char *motd);
	bool supressChat;
	void SupressChat();

private:
	PlayerBool *bools_front;
	PlayerBool *bools_back;
	
	PlayerNumber *numbers_front;
	PlayerNumber *numbers_back;
	
	PlayerString *strings_front;
	PlayerString *strings_back;

	bool has_map;
	bool is_initialized;
	bool self_destruct;

	unsigned char id;
	bool op;
	bool isVisible;
	bool isHidden;
	
	char *name;
	position pos;
	position newpos;
	Position savedpos;
	block blockpos;
	SOCKET sock;

	char lastChat[65];

	char *getPacket();
	static void playerThread(void *params);
	
	void handlePacket(const char *buffer);
	void handleLoginPacket(const char *buffer);
	void handleChatPacket(const char *buffer);
	void handleMovementPacket(const char *buffer);
	void handleBlockPacket(const char *buffer);

	bool sendKick(const char *msg);
	bool sendWelcome();
	bool sendLevel(Level *lvl);
	bool sendSpawns();

	bool sendPacket(const char *packet, int length);

	bool validNonLoginPacket(const char PACK_CODE);

	Server *server;
	std::vector<class Player*> localPlayerList;

	void SendPackets();
};

#endif