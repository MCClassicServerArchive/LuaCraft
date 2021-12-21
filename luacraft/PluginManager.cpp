#include <cstdlib>
#include "Player.h"
#include "Various.h"
#include "Console.h"
#include "tolua++.h"
#include "Plugin_Lua.h"
#include "Server.h"
#include "Server_Lua.h"
#include "Player_Lua.h"
#include "Structs_Lua.h"
#include "Structs.h"
#include "Level.h"
#include "Level_Lua.h"
#include "PluginManager.h"

#pragma comment(lib, "toLua++.lib")

PendingHook::PendingHook(char *_hook, Player *_Player) {
	this->hook = (char*)malloc(strlength(_hook)+1);
		/* +1 to make space for the null-terminator */
	strcpy(this->hook, _hook);

	this->player = _Player;

	this->next = NULL;
}

Plugin::Plugin(char *_name, int _id, PluginManager *_plugins) {
	this->name = (char*)malloc(strlength(_name));
	strcpy(this->name, _name);

	this->id = _id;

	this->plugins = _plugins;

	this->RequiresUnload = false;

	this->next = NULL;
}

PluginManager::PluginManager(Server *_server) {
	this->server = _server;

	this->pendinghooks_back = NULL;
	this->pendinghooks_front = NULL;

	this->hooks_back = NULL;
	this->hooks_front = NULL;

	this->plugins_back = NULL;
	this->plugins_front = NULL;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PluginManagerThread, (void*)this, 0, NULL);
}

PluginManager::~PluginManager() {
	lua_close(this->Lua);
}

void PluginManager::CallHook(char *hook, Player *player) {
	PendingHook *newHook = new PendingHook(hook, player);
	
	if(!(this->pendinghooks_front || this->pendinghooks_back)) {
		this->pendinghooks_back = newHook;
		this->pendinghooks_front = newHook;
	} else {
		this->pendinghooks_back->next = newHook;
		this->pendinghooks_back = newHook;
	}
}

void PluginManager::PluginManagerThread(void *param) {
	PluginManager *_this = (PluginManager*)param;
	PendingHook *oldHook;

	while(true) {
		while(_this->pendinghooks_front) {
			oldHook = _this->pendinghooks_front;
			_this->RunHook(oldHook);

			if(strcmp(oldHook->hook, "OnPlayerLeave") == 0)
				oldHook->player->CanDelete = true;

			if(oldHook->next)
				_this->pendinghooks_front = oldHook->next;
			else {
				_this->pendinghooks_front = NULL;
				_this->pendinghooks_back = NULL;
			}
			free(oldHook->hook);
			delete oldHook;
			/*I cleaned up the code a bit here. Compare with rev 70*/
		
		}
		Sleep(5);
	}
}

void PluginManager::LoadGamemode(char *gamemodeName) {
	// Get filename
	char gamemodeFile[128];
	sprintf(gamemodeFile, "gamemodes/%s/init.lua", gamemodeName);

	// Print to console
	Console::PrintText("Loading gamemode: %s", gamemodeFile);

	// Create new Plugin class
	Plugin *newPlugin = new Plugin(gamemodeName, 0, this);
	this->plugins_back = newPlugin;
	this->plugins_front = newPlugin;

	// Set GAMEMODE and SERVER
	tolua_pushusertype(Lua, (void*)newPlugin, "Plugin");
	lua_setfield(Lua, LUA_GLOBALSINDEX, "GAMEMODE");
	tolua_pushusertype(Lua, (void*)this->server, "Server");
	lua_setfield(Lua, LUA_GLOBALSINDEX, "SERVER");

	// Get _PLUGINS on stack
	lua_getfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");

	// Push ID for _PLUGINS index onto stack
	lua_pushnumber(Lua, newPlugin->id);

	// Create new environment for plugin
	lua_newtable(Lua);

	// Create metatable for environment
	lua_newtable(Lua);
	lua_getfield(Lua, LUA_GLOBALSINDEX, "_G");
	lua_setfield(Lua, -2, "__index");
	lua_setmetatable(Lua, -2);

	// Store environment in _PLUGINS[id]
	lua_settable(Lua, 1);

	// Try to load file onto stack
	if(luaL_loadfile(Lua, gamemodeFile)) {
		Console::PrintText("ERROR LOADING GAMEMODE: %s", lua_tostring(Lua, -1));
		lua_settop(Lua, 0);
		return;
	}

	// Load custom lua chunk, set environment for it, and try to exec it
	luaL_loadstring(Lua, "function print(a) GAMEMODE:Print(a) end; function include(a) GAMEMODE:Include(a) end");
	this->SetEnvironment(newPlugin);
	try {
		if(lua_pcall(Lua, 0, 0, 0)) {
			Console::PrintText("LUA ERROR: %s", lua_tostring(Lua, -1));
		}
	} catch(...) {
		Console::PrintText("EXCEPTION: In lua_pcall for gamemode starting chunk '%s'", newPlugin->name);
	}

	// Set the environment
	this->SetEnvironment(newPlugin);

	// Try to execute the plugin
	try {
		if(lua_pcall(Lua, 0, 0, 0)) {
			Console::PrintText("LUA ERROR: %s", lua_tostring(Lua, -1));
		}
	} catch(...) {
		Console::PrintText("EXCEPTION: In lua_pcall for gamemode");
	}

	// Clear the stack
	lua_settop(Lua, 0);
}

void PluginManager::LoadPlugin(char *pluginName) {
	// Get filename
	char pluginFile[128];
	sprintf(pluginFile, "plugins/%s.lua", pluginName);

	// Create new Plugin class
	Plugin *newPlugin = new Plugin(pluginName, this->plugins_back->id+1, this);
	this->plugins_back->next = newPlugin;
	this->plugins_back = newPlugin;

	// Get _PLUGINS on stack
	lua_getfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");

	// Push ID for _PLUGINS index onto stack
	lua_pushnumber(Lua, newPlugin->id);

	// Create new environment for plugin
	lua_newtable(Lua);

	// Set PLUGIN and SERVER
	tolua_pushusertype(Lua, (void*)newPlugin, "Plugin");
	lua_setfield(Lua, -2, "PLUGIN");
	tolua_pushusertype(Lua, (void*)this->server, "Server");
	lua_setfield(Lua, -2, "SERVER");

	// Create metatable for environment
	lua_newtable(Lua);
	lua_getfield(Lua, LUA_GLOBALSINDEX, "_G");
	lua_setfield(Lua, -2, "__index");
	lua_setmetatable(Lua, -2);

	// Store environment in _PLUGINS[id]
	lua_settable(Lua, 1);

	// Try to load file onto stack
	if(luaL_loadfile(Lua, pluginFile)) {
		Console::PrintText("ERROR LOADING PLUGIN ('%s'): %s", newPlugin->name, lua_tostring(Lua, -1));
		lua_pop(Lua, 1);
	}

	// Load custom lua chunk, set environment for it, and try to exec it
	luaL_loadstring(Lua, "function print(a) PLUGIN:Print(a) end;");
	this->SetEnvironment(newPlugin);
	try {
		if(lua_pcall(Lua, 0, 0, 0)) {
			Console::PrintText("LUA ERROR: %s", lua_tostring(Lua, -1));
			lua_pop(Lua, 1);
		}
	} catch(...) {
		Console::PrintText("EXCEPTION: In lua_pcall for plugin starting chunk '%s'", newPlugin->name);
	}

	// Set the environment
	this->SetEnvironment(newPlugin);

	// Try to execute the plugin
	try {
		if(lua_pcall(Lua, 0, 0, 0)) {
			Console::PrintText("LUA ERROR: %s", lua_tostring(Lua, -1));
			lua_pop(Lua, 1);
		}
	} catch(...) {
		Console::PrintText("EXCEPTION: In lua_pcall for plugin '%s'", newPlugin->name);
	}

	// Clear the stack
	//lua_settop(Lua, 0);

	// Print loaded message
	Console::PrintText("LOADED: %s", newPlugin->name);
}

bool PluginManager::SetEnvironment(Plugin *_plugin) {
	lua_getfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");
	lua_pushnumber(Lua, _plugin->id);
	lua_gettable(Lua, -2);
	lua_setfenv(Lua, -3);
	lua_pop(Lua, 1);

	return true;
}

void PluginManager::InitLua() {
	this->Lua = lua_open();
	if(!Lua) {
		Console::PrintText("ERROR: Could not initialize Lua!");
		return;
	}

	lua_newtable(Lua);
	lua_setfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");

	luaL_openlibs(Lua);

	tolua_Plugin_open(Lua);
	tolua_Server_open(Lua);
	tolua_Player_open(Lua);
	tolua_Structs_open(Lua);
	tolua_Level_open(Lua);

	lua_register(Lua, "GetPluginByName", &PluginManager::GetPluginByName);

	Console::PrintText("Initialized Lua");
}

void PluginManager::LoadPlugins() {
	int pluginsLoaded = 0;
	FILE *pluginsList;
	pluginsList = fopen("plugins.txt","r");
	if(pluginsList == NULL) {
		pluginsList = fopen("plugins.txt","w");
		fputs("# Add the name of your plugin below, one per line.\n", pluginsList);
		fputs("# Example: Teleport\n", pluginsList);
		fputs("# That example would load the plugin: plugins/Teleport.lua\n", pluginsList);
		fputs("# Anything after a # is ignored.", pluginsList);
		fclose(pluginsList);
		pluginsList = fopen("plugins.txt","r");
	}
	char fileNameBuffer[128];
	char lineBuffer[128];
	while(!feof(pluginsList)) {
		fgets(lineBuffer, sizeof(lineBuffer), pluginsList);
		for(int i=0; i<sizeof(lineBuffer); i++) {
			if(lineBuffer[i] == '\n' || lineBuffer[i] == '#')
				lineBuffer[i] = '\0';
		}
		char *chr;
		chr = strchr(lineBuffer, ' ');
		while(chr != NULL) {
			strcpy(chr, chr+1);
			chr = strchr(lineBuffer, ' ');
		}
		if(lineBuffer[0] == '\0')
			continue;
		this->LoadPlugin(lineBuffer);
		pluginsLoaded++;
	}
	fclose(pluginsList);
	if(!pluginsLoaded)
		Console::PrintText("No plugins listed in Plugins.txt");
}

void PluginManager::RunHook(PendingHook *pendingHook) {
	Hook *hook = this->hooks_front;
	int numArgs;

	while(true) {
		// If hook is invalid, return
		if(!hook) break;

		// Check if hooked event is the same as the event called
		if(strcmp(hook->hook, pendingHook->hook)) {
			hook = hook->next;
			continue;
		}

		// Clear stack
		lua_settop(Lua, 0);

		// Set numArgs to 0
		numArgs = 0;

		// Get _PLUGINS
		lua_getfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");

		// Get environment instance
		lua_pushnumber(Lua, hook->plugin->id);
		lua_gettable(Lua, -2);

		// Get function
		lua_getfield(Lua, -1, hook->function);

		// Set environment
		if(!this->SetEnvironment(hook->plugin)) {
			this->CallHook(pendingHook->hook, pendingHook->player);
			lua_settop(Lua, 0);
			hook = hook->next;
			continue;
		}
		
		// If player, then push player
		if(pendingHook->player) {
			tolua_pushusertype(Lua, (void*)pendingHook->player, "Player");
			numArgs++;
		}

		// Try to run the function
		try {
			if(lua_pcall(Lua, numArgs, 0, 0)) {
				Console::PrintText("LUA ERROR: %s", lua_tostring(Lua, -1));
			}
		} catch(...) {
			DumpStack();
			Console::PrintText("EXCEPTION: In lua_pcall for hook '%s' in '%s' {%d}", hook->hook, hook->plugin->name, __LINE__);
		}

		// Clear stack
		lua_settop(Lua, 0);

		// Next hook
		hook = hook->next;
	}

	this->CheckForUnloads();
}

void PluginManager::CheckForUnloads() {
	Plugin *p = this->plugins_front;
	Plugin *oldP;
	
	while(true) {
		if(!p) return;
		if(!p->next) return;
		if(p->next->RequiresUnload) {
			oldP = p->next;
			p->next = oldP->next;
			this->RemoveHooks(oldP);
			delete oldP;
			return;
		}
		p = p->next;
	}
}

void PluginManager::AddHook(const char *_hook, const char *_function, Plugin *_plugin) {
	Hook *newHook = new Hook();

	newHook->function = (char*)malloc(strlength(_function));
	strcpy(newHook->function, _function);

	newHook->hook = (char*)malloc(strlength(_hook));
	strcpy(newHook->hook, _hook);

	newHook->plugin = _plugin;
	newHook->next = NULL;

	if(this->hooks_front == NULL || this->hooks_back == NULL) {
		this->hooks_back = newHook;
		this->hooks_front = newHook;
	} else {
		this->hooks_back->next = newHook;
		this->hooks_back = newHook;
	}
}

char *PluginManager::GetGamemodeName() {
	return this->plugins_front->name;
}

int PluginManager::GetPluginByName(lua_State *Lua) {
	lua_getfield(Lua, LUA_GLOBALSINDEX, "SERVER");
	Server *server;
	server = (Server*)tolua_tousertype(Lua, -1, 0);
	lua_pop(Lua, 1);
	PluginManager *_this = server->plugins;

	Plugin *p = _this->plugins_front;

	char *pluginName = (char*)lua_tostring(Lua, -1);

	while(true) {
		if(!p) {
			lua_pushnil(Lua);
			return 1;
		}
		if(!strcmp(p->name, pluginName)) {
			lua_getfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");
			lua_pushnumber(Lua, p->id);
			lua_gettable(Lua, -2);
			lua_remove(Lua, -2);
			return 1;
		}
		p = p->next;
	}
}

void PluginManager::DumpStack() {
	int i=lua_gettop(Lua);
	printf(" ----------------  Stack Dump ----------------\n" );
	while(i) {
		int t = lua_type(Lua, i);
		switch (t) {
			case LUA_TSTRING:
				printf("%d:`%s'\n", i, lua_tostring(Lua, i));
				break;
			case LUA_TBOOLEAN:
				printf("%d: %s\n",i,lua_toboolean(Lua, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:
				 printf("%d: %g\n",  i, lua_tonumber(Lua, i));
				break;
				default: printf("%d: %s\n", i, lua_typename(Lua, t)); break;
		}
		i--;
	}
	printf("--------------- Stack Dump Finished ---------------\n" );
}

void PluginManager::Unload(Plugin *_plugin) {
	_plugin->RequiresUnload = true;
}

void PluginManager::RemoveHooks(Plugin *_plugin) {
	Hook *h = this->hooks_front;
	Hook *oldH;

	while(true) {
		if(!h) return;
		if(!h->next) return;
		if(h->next->plugin == _plugin) {
			oldH = h->next;
			h->next = oldH->next;
			delete oldH;
		}
		h = h->next;
	}
}

// ===========================
// Lua functions for Plugin:
// ===========================

void Plugin::Print(const char *msg) {
	if(this->id)
		Console::PrintText("[%s] %s", this->name, msg);
	else
		Console::PrintText(msg);
}

void Plugin::AddHook(const char *_hook, const char *_function) {
	this->plugins->AddHook(_hook, _function, this);
}

void Plugin::SetName(const char *_name) {
	this->name = (char*)malloc(strlength(_name));
	strcpy(this->name, _name);
}

char *Plugin::GetName() {
	return this->name;
}

void Plugin::Include(const char *_file) {
	// Get include filename
	char includeFile[128];
	sprintf(includeFile, "gamemodes/%s/%s", this->name, _file);

	// Try to load file onto stack
	if(luaL_loadfile(this->plugins->Lua, includeFile)) {
		Console::PrintText("LUA ERROR: %s", lua_tostring(this->plugins->Lua, -1));
		lua_settop(this->plugins->Lua, 0);
		return;
	}

	// Set the environment
	this->plugins->SetEnvironment(this);

	// Try to execute the file
	try {
		if(lua_pcall(this->plugins->Lua, 0, 0, 0)) {
			Console::PrintText("LUA ERROR: %s", lua_tostring(this->plugins->Lua, -1));
		}
	} catch(...) {
		Console::PrintText("EXCEPTION: In lua_pcall for include for '%s'", this->name);
	}
}

void Plugin::Unload() {
	this->plugins->Unload(this);
}