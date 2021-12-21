#ifndef _PLUGIN_MANAGER_H
#define _PLUGIN_MANAGER_H
/*	============================================
		The PluginManager class is for holding
		and calling hooks, and managing the
		plugins
	============================================ */

#include "Defines.h"
#include "Various.h"

#pragma comment(lib, "lua/lua51.lib")

extern "C" {
	#include "lua/lua.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
}

class Server;
class Player;

class Plugin;
struct PendingHook;
struct Hook;

class PluginManager {
public:
	PluginManager(Server *_server);
	~PluginManager();

	void AddHook(const char *_hook, const char *_function, Plugin *_plugin);
	void CallHook(char *hook, Player *player);

	void LoadGamemode(char *gamemodeName);
	void LoadPlugin(char *pluginName);
	void InitLua();
	void LoadPlugins();

	char *GetGamemodeName();
	static int GetPluginByName(lua_State *Lua);

	void Unload(Plugin *_plugin);
	void RemoveHooks(Plugin *_plugin);

	bool SetEnvironment(Plugin *_plugin);

	lua_State *Lua;

private:
	PendingHook *pendinghooks_front;
	PendingHook *pendinghooks_back;

	Plugin *plugins_front;
	Plugin *plugins_back;

	Hook *hooks_front;
	Hook *hooks_back;

	Server *server;

	void DumpStack();

	void RunHook(PendingHook *pendingHook);

	static void PluginManagerThread(void *param);

	void CheckForUnloads();
};

struct PendingHook {
	char *hook;
	Player *player;

	PendingHook *next;

	PendingHook(char *_hook, Player *_Player);
};

class Plugin {
public:
	char *name;
	int id;

	bool RequiresUnload;
	
	Plugin *next;

	Plugin(char *_name, int _id, PluginManager *_plugins);

	// Lua functions:

	void Print(const char* msg);
	void AddHook(const char *_hook, const char *_function);
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
};

#endif