#ifndef _PLUGIN_MANAGER_H
#define _PLUGIN_MANAGER_H
/*	============================================
		The PluginManager class is for holding
		and calling hooks, and managing the
		plugins
	============================================ */

#include "Various.h"
#include "Zone.h"

#pragma comment(lib, "lua/lua51.lib")

extern "C" {
	#include "lua/lua.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
}

#ifndef _WIN32
	#include <pthread.h>
#endif

class Server;
class Player;

class Plugin;
struct PendingHook;
struct Hook;
struct ChatCommand;
struct Timeout;

class PluginManager {
public:
	PluginManager(Server *_server);
	~PluginManager();

	void AddHook(const char *_hook, const char *_function, Plugin *_plugin);
	void AddChatCommand(const char *_command, const char *_function, Plugin *_plugin);
	void SetTimeout(int seconds, const char *_function, Plugin *_plugin);
	void RemoveTimeout(Timeout *_timeout);
	void CallHook(char *hook, Player *player);

	void LoadGamemode(char *gamemodeName);
	void LoadPlugin(char *pluginName);
	void InitLua();
	void LoadPlugins();

	char *GetGamemodeName();
	static int GetPluginByName(lua_State *Lua);
	static int GetPlayers(lua_State *Lua);
	static int GetPlayersInWorld(lua_State *Lua);
	static int GetBlocksOfType(lua_State *Lua);

	void Unload(Plugin *_plugin);
	void RemoveHooks(Plugin *_plugin);

	bool SetEnvironment(Plugin *_plugin);

	Zone *CreateZone(Level *_level, block _block1, block _block2);
	bool RemoveZone(Zone *zone);

	lua_State *Lua;

private:
	PendingHook *pendinghooks_front;
	PendingHook *pendinghooks_back;

	Timeout *timeouts_front;
	Timeout *timeouts_back;

	Timeout *timeouts_pool_front;
	Timeout *timeouts_pool_back;

	Plugin *plugins_front;
	Plugin *plugins_back;

	Hook *hooks_front;
	Hook *hooks_back;

	ChatCommand *chatcommands_front;
	ChatCommand *chatcommands_back;

	Zone *zones_front;
	Zone *zones_back;

	Server *server;

	time_t NextTick;
	PendingHook *TickHook;

	void setGlobals();

	void DumpStack();

	void RunHook(PendingHook *pendingHook);
	void RunChatCommand(ChatCommand *command, Player *player);
	void RunTimeout(Timeout *pendingTimeout);

	static void PluginManagerThread(void *param);

	void CheckForUnloads();
};

struct PendingHook {
	char *hook;
	Player *player;

	PendingHook *next;

	PendingHook(char *_hook, Player *_Player);
	~PendingHook();
};

class Plugin {
public:
	char *name;
	int id;

	bool RequiresUnload;
	
	Plugin *next;

	Plugin(char *_name, int _id, PluginManager *_plugins);
	~Plugin();

	// Lua functions:

	void Print(const char* msg);
	void AddHook(const char *_hook, const char *_function);
	void AddChatCommand(const char *_command, const char *_function);
	void SetTimeout(int seconds, const char *_function);
	void SetName(const char *_name);
	char *GetName();
	void Include(const char *_file);
	void Unload();

private:
	PluginManager *plugins;
};

struct Hook {
	char *hook;
	char *function;
	Plugin *plugin;

	Hook *next;

	inline ~Hook() {
		free(this->function);
		free(this->hook);
	}
};

struct ChatCommand {
	char *command;
	char *function;
	Plugin *plugin;

	ChatCommand *next;

	inline ~ChatCommand() {
		free(this->command);
		free(this->function);
	}
};

struct Timeout {
	time_t time;
	char *function;
	Plugin *plugin;

	Timeout *next;
	bool active;

	inline ~Timeout() {
		free(this->function);
	}
};

#endif