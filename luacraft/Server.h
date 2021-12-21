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
#include "Zone.h"
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
	bool registerPlayer(class Player *player);
	void initPlayer(class Player *player, NetworkReturn *Ret);
	class Player* GetPlayerByName(const char *name);
	bool Quit;
	class Network *getNetwork();
	int getSalt();

	void BeatMinecraft();
	void BeatFlist();
	
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
	void SendMove(Player *player);
	void PlaceBlock(block nblock, char* world);
	block GetBlock(char *world, short x, short y, short z);
	std::vector<block> *GetBlocksOfType(char *world, char type);
	
	int GetWorldSizeX(char *world);
	int GetWorldSizeY(char *world);
	int GetWorldSizeZ(char *world);

	void DespawnPlayer(Player *player);
	void RespawnPlayer(Player *player);

	PluginManager *plugins;

	char *GetGamemodeName();
	void GetPluginByName(const char *pluginName);
	void LoadPlugin(const char *fileName);

	char *GetName();
	char *GetMotd();
	void SetName(const char *name);
	void SetMotd(const char *motd);

	int NumPlayers();
	int NumPlayersInWorld(const char *world);
	int MaxPlayers();

	std::vector<Player*> GetPlayers();
	std::vector<Player*> GetPlayersInWorld(const char *world);

	Zone *CreateZone(char *_world, block _block1, block _block2);
	bool DeleteZone(Zone *_zone);

private:

	bool CanBeatFlist;
	char *server_name, *server_motd, *server_port, *server_hash, *server_website;
	bool server_public, server_verify_names;
	int max_players;

	Network *network;
	unsigned char getFreePlayerId();
	long salt;

	time_t timer, time_flist, time_mcnet;

	class Player* getPlayerById(unsigned char id);

	HANDLE srvMutex;
	HANDLE mainListMutex;
};

#endif