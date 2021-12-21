#ifndef _SERVER_H
#define _SERVER_H
/*	============================================
		The Server class will only have one
		instance, and is created in main().
		It contains the main loop, and directs
		the operation of everything server-side.
	============================================ */

#include "Console.h"
#include "PluginManager.h"
#include "Network.h"
#include "Level.h"
#include <vector>

class Network;
class Level;
class PluginManager;

class Server {
public:
	Server();
	~Server();

	bool Step();
	bool releasePlayer(class Player *player);
	bool addPlayer(class Player *player);
	class Player* GetPlayerByName(const char *name);
	bool Quit;
	class Network *getNetwork();
	int getSalt();
	
	std::vector<Player*> playerList;

	void CreateWorld(const char *worldName, const char *fileName);
	Level *GetLevelByName(const char *worldName);
	void DeleteWorld(const char *worldName);
	std::vector<Level*> worldList;

	char *getServerName();
	char *getServerMotd();
	bool getVerifyNames();

	void SendChat(const char *message);
	void SendChat(const char *message, const char *world);
	void sendMove(Player *player);
	void sendBlock(block nblock, char* world);

	void DespawnPlayer(Player *player);
	void RespawnPlayer(Player *player);

	PluginManager *plugins;

	char *GetGamemodeName();
	void GetPluginByName(const char *pluginName);
	void LoadPlugin(const char *fileName);

private:

	char *server_name, *server_motd, *server_port;
	bool server_public, server_verify_names;
	int max_players;

	Network *network;
	unsigned char getFreePlayerId();
	int salt;

	class Player* getPlayerById(unsigned char id);

	HANDLE srvMutex;
};

#endif