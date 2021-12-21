#include <cstdlib>

#include "Defines.h"
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
#include "Zone_Lua.h"
#include "PluginManager.h"

#pragma comment(lib, "toLua++.lib")

PendingHook::PendingHook(char *_hook, Player *_Player) {
	this->hook = (char*)malloc(strlength(_hook)+1);

	strcpy(this->hook, _hook);

	this->player = _Player;

	this->next = NULL;
}

PendingHook::~PendingHook() {
	if(strcmp(this->hook, "PlayerShouldChat"))
		free(this->hook);
}

Plugin::Plugin(char *_name, int _id, PluginManager *_plugins) {
	this->name = (char*)malloc(strlength(_name)+1);
	strcpy(this->name, _name);

	this->id = _id;

	this->plugins = _plugins;

	this->RequiresUnload = false;

	this->next = NULL;
}

Plugin::~Plugin() {
	free(this->name);
}

PluginManager::PluginManager(Server *_server) {
	this->server = _server;

	this->pendinghooks_back = NULL;
	this->pendinghooks_front = NULL;

	this->hooks_back = NULL;
	this->hooks_front = NULL;

	this->chatcommands_back = NULL;
	this->chatcommands_front = NULL;

	this->plugins_back = NULL;
	this->plugins_front = NULL;

	this->timeouts_back = NULL;
	this->timeouts_front = NULL;

	this->timeouts_pool_back = NULL;
	this->timeouts_pool_front = NULL;

	this->zones_back = NULL;
	this->zones_front = NULL;

	this->NextTick = time(0) + 1;
	this->TickHook = new PendingHook("OnTick", NULL);
	this->TickHook->next = NULL;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PluginManagerThread, (void*)this, 0, NULL);
}

PluginManager::~PluginManager() {
	lua_close(this->Lua);
	delete this->TickHook;
}

void PluginManager::CallHook(char *hook, Player *player) {
	//WaitForSingleObject(CallMutex, INFINITE);
	PendingHook *newHook = new PendingHook(hook, player);
	
	try {
		if(!(this->pendinghooks_front || this->pendinghooks_back)) {
			this->pendinghooks_back = newHook;
			this->pendinghooks_front = newHook;
		} else {
			this->pendinghooks_back->next = newHook;
			this->pendinghooks_back = newHook;
		}
	} catch(...) {}
	//ReleaseMutex(CallMutex);
}

void PluginManager::PluginManagerThread(void *param) {
	PluginManager *_this = (PluginManager*)param;
	PendingHook *oldHook;
	Timeout *oldTimeout;
	char *cmd;
	char *cmdChr;

	while(true) {
		while(_this->pendinghooks_front) {
			oldHook = _this->pendinghooks_front;

			if(!strcmp(oldHook->hook, "OnPlayerChat")) {
				oldHook->player->supressChat = false;
				cmd = (char*)malloc(strlength(oldHook->player->GetLastChat())+1);
				strcpy(cmd, oldHook->player->GetLastChat());
				makeLower(cmd);
				for(cmdChr = cmd; cmdChr[0]; cmdChr++) {
					if(cmdChr[0] == 0x20) {
						cmdChr[0] = 0x00;
						break;
					}
				}
				for(ChatCommand *c = _this->chatcommands_front; c; c = c->next) {
					if(strcmp(cmd, c->command))
						continue;
					oldHook->player->SupressChat();
					_this->RunChatCommand(c, oldHook->player);
				}
			}

			_this->RunHook(oldHook);

			if(!strcmp(oldHook->hook, "OnPlayerLeave"))
				oldHook->player->CanDelete = true;

			if((!strcmp(oldHook->hook, "OnPlayerChat")) && (!oldHook->player->supressChat)) {
				free(oldHook->hook);
				oldHook->hook = "PlayerShouldChat";
				_this->RunHook(oldHook);
			}

			if(oldHook->next)
				_this->pendinghooks_front = oldHook->next;
			else {
				_this->pendinghooks_front = NULL;
				_this->pendinghooks_back = NULL;
			}

			delete oldHook;		
		}
		time_t curTime;
		Timeout *tempTimeout;
		for(Timeout *t = _this->timeouts_front; t;) {
			curTime = time(0);
			if(t->time < curTime) {
				tempTimeout = t->next;
				_this->RunTimeout(t);
				_this->RemoveTimeout(t);
				t = tempTimeout;
			} else
				t = t->next;
		}
		curTime = time(0);
		if(_this->NextTick < curTime) {
			_this->RunHook(_this->TickHook);
			_this->NextTick = curTime + 1;
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
	setGlobals();

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
	luaL_loadstring(Lua, "function print(a) GAMEMODE:Print(a) end;\
						 function include(a) GAMEMODE:Include(a) end;\
						 SERVER.GetPlayers = GetPlayers;\
						 SERVER.GetPlayersInWorld = GetPlayersInWorld;\
						 SERVER.GetPluginByName = GetPluginByName;\
						 SERVER.GetBlocksOfType = GetBlocksOfType;");
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
	setGlobals();

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
	luaL_loadstring(Lua, "function print(a) PLUGIN:Print(a) end;\
						 SERVER.GetPlayers = GetPlayers;\
						 SERVER.GetPlayersInWorld = GetPlayersInWorld;\
						 SERVER.GetPluginByName = GetPluginByName;\
						 SERVER.GetBlocksOfType = GetBlocksOfType;");
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
	tolua_Zone_open(Lua);

	lua_register(Lua, "GetPluginByName", &PluginManager::GetPluginByName);
	lua_register(Lua, "GetPlayers", &PluginManager::GetPlayers);
	lua_register(Lua, "GetPlayersInWorld", &PluginManager::GetPlayersInWorld);
	lua_register(Lua, "GetBlocksOfType", &PluginManager::GetBlocksOfType);

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
			try {
				tolua_pushusertype(Lua, (void*)pendingHook->player, "Player");
				numArgs++;
			} catch(...) {
				Console::PrintText("Exception for pushing player!!!");
			}
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

void PluginManager::RunChatCommand(ChatCommand *command, Player *player) {
	// Clear stack
	lua_settop(Lua, 0);

	// Get _PLUGINS
	lua_getfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");

	// Get environment instance
	lua_pushnumber(Lua, command->plugin->id);
	lua_gettable(Lua, -2);

	// Get function
	lua_getfield(Lua, -1, command->function);

	// Set environment
	if(!this->SetEnvironment(command->plugin)) {
		lua_settop(Lua, 0);
		return;
	}
	
	// Push player
	tolua_pushusertype(Lua, (void*)player, "Player");

	// Push text that comes after the command
	if(strlength(player->GetLastChat()) == strlength(command->command))
		lua_pushnil(Lua);
	else
		lua_pushstring(Lua, player->GetLastChat() + strlength(command->command) + 1);

	// Try to run the function
	try {
		if(lua_pcall(Lua, 2, 0, 0)) {
			Console::PrintText("LUA ERROR: %s", lua_tostring(Lua, -1));
		}
	} catch(...) {
		DumpStack();
		Console::PrintText("EXCEPTION: In lua_pcall for chat command '%s' in '%s' {%d}", command->command, command->plugin->name, __LINE__);
	}

	this->CheckForUnloads();
}

void PluginManager::RunTimeout(Timeout *pendingTimeout) {
	// Clear stack
	lua_settop(Lua, 0);

	// Get _PLUGINS
	lua_getfield(Lua, LUA_GLOBALSINDEX, "_PLUGINS");

	// Get environment instance
	lua_pushnumber(Lua, pendingTimeout->plugin->id);
	lua_gettable(Lua, -2);

	// Get function
	lua_getfield(Lua, -1, pendingTimeout->function);

	// Set environment
	this->SetEnvironment(pendingTimeout->plugin);

	// Try to run the function
	try {
		if(lua_pcall(Lua, 0, 0, 0)) {
			Console::PrintText("LUA ERROR: %s", lua_tostring(Lua, -1));
		}
	} catch(...) {
		DumpStack();
		Console::PrintText("EXCEPTION: In lua_pcall for timeout in '%s' {%d}", pendingTimeout->plugin->name, __LINE__);
	}

	// Clear stack
	lua_settop(Lua, 0);

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

	newHook->function = (char*)malloc(strlength(_function)+1);
	strcpy(newHook->function, _function);

	newHook->hook = (char*)malloc(strlength(_hook)+1);
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

void PluginManager::AddChatCommand(const char *_command, const char *_function, Plugin *_plugin) {
	makeLower((char*)_command);
	ChatCommand *newChatCommand = new ChatCommand();

	newChatCommand->function = (char*)malloc(strlength(_function)+1);
	strcpy(newChatCommand->function, _function);

	newChatCommand->command = (char*)malloc(strlength(_command)+1);
	strcpy(newChatCommand->command + 1, _command);
	newChatCommand->command[0] = '/';

	newChatCommand->plugin = _plugin;
	newChatCommand->next = NULL;

	if(this->chatcommands_front == NULL || this->chatcommands_back == NULL) {
		this->chatcommands_back = newChatCommand;
		this->chatcommands_front = newChatCommand;
	} else {
		this->chatcommands_back->next = newChatCommand;
		this->chatcommands_back = newChatCommand;
	}
}

char *PluginManager::GetGamemodeName() {
	if(this->plugins_front)
		return this->plugins_front->name;
	else
		return NULL;
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

void PluginManager::setGlobals() {
	lua_pushinteger(Lua, 0);
	lua_setglobal(Lua, "BLOCK_AIR");
	lua_pushinteger(Lua, 1);
	lua_setglobal(Lua, "BLOCK_STONE");
	lua_pushinteger(Lua, 2);
	lua_setglobal(Lua, "BLOCK_GRASS");
	lua_pushinteger(Lua, 3);
	lua_setglobal(Lua, "BLOCK_DIRT");
	lua_pushinteger(Lua, 4);
	lua_setglobal(Lua, "BLOCK_COBBLE");
	lua_pushinteger(Lua, 4);
	lua_setglobal(Lua, "BLOCK_COBBLESTONE");
	lua_pushinteger(Lua, 5);
	lua_setglobal(Lua, "BLOCK_WOOD");
	lua_pushinteger(Lua, 5);
	lua_setglobal(Lua, "BLOCK_PLANK");
	lua_pushinteger(Lua, 6);
	lua_setglobal(Lua, "BLOCK_PLANT");
	lua_pushinteger(Lua, 7);
	lua_setglobal(Lua, "BLOCK_ROCK");
	lua_pushinteger(Lua, 7);
	lua_setglobal(Lua, "BLOCK_OPCRETE");
	lua_pushinteger(Lua, 8);
	lua_setglobal(Lua, "BLOCK_WATER");
	lua_pushinteger(Lua, 9);
	lua_setglobal(Lua, "BLOCK_SWATER");
	lua_pushinteger(Lua, 10);
	lua_setglobal(Lua, "BLOCK_LAVA");
	lua_pushinteger(Lua, 11);
	lua_setglobal(Lua, "BLOCK_SLAVA");
	lua_pushinteger(Lua, 12);
	lua_setglobal(Lua, "BLOCK_SAND");
	lua_pushinteger(Lua, 13);
	lua_setglobal(Lua, "BLOCK_GRAVEL");
	lua_pushinteger(Lua, 14);
	lua_setglobal(Lua, "BLOCK_GOLDORE");
	lua_pushinteger(Lua, 15);
	lua_setglobal(Lua, "BLOCK_IRONORE");
	lua_pushinteger(Lua, 16);
	lua_setglobal(Lua, "BLOCK_COAL");
	lua_pushinteger(Lua, 17);
	lua_setglobal(Lua, "BLOCK_TREE");
	lua_pushinteger(Lua, 17);
	lua_setglobal(Lua, "BLOCK_TRUNK");
	lua_pushinteger(Lua, 18);
	lua_setglobal(Lua, "BLOCK_LEAVES");
	lua_pushinteger(Lua, 19);
	lua_setglobal(Lua, "BLOCK_SPONGE");
	lua_pushinteger(Lua, 20);
	lua_setglobal(Lua, "BLOCK_GLASS");
	lua_pushinteger(Lua, 21);
	lua_setglobal(Lua, "BLOCK_RED");
	lua_pushinteger(Lua, 22);
	lua_setglobal(Lua, "BLOCK_ORANGE");
	lua_pushinteger(Lua, 23);
	lua_setglobal(Lua, "BLOCK_YELLOW");
	lua_pushinteger(Lua, 24);
	lua_setglobal(Lua, "BLOCK_LGREEN");
	lua_pushinteger(Lua, 24);
	lua_setglobal(Lua, "BLOCK_LIGHTGREEN");
	lua_pushinteger(Lua, 25);
	lua_setglobal(Lua, "BLOCK_GREEN");
	lua_pushinteger(Lua, 26);
	lua_setglobal(Lua, "BLOCK_AGREEN");
	lua_pushinteger(Lua, 26);
	lua_setglobal(Lua, "BLOCK_AQUAGREEN");
	lua_pushinteger(Lua, 27);
	lua_setglobal(Lua, "BLOCK_CYAN");
	lua_pushinteger(Lua, 28);
	lua_setglobal(Lua, "BLOCK_BLUE");
	lua_pushinteger(Lua, 29);
	lua_setglobal(Lua, "BLOCK_PURPLE");
	lua_pushinteger(Lua, 30);
	lua_setglobal(Lua, "BLOCK_INDIGO");
	lua_pushinteger(Lua, 31);
	lua_setglobal(Lua, "BLOCK_VIOLET");
	lua_pushinteger(Lua, 32);
	lua_setglobal(Lua, "BLOCK_MAGENTA");
	lua_pushinteger(Lua, 33);
	lua_setglobal(Lua, "BLOCK_PINK");
	lua_pushinteger(Lua, 34);
	lua_setglobal(Lua, "BLOCK_BLACK");
	lua_pushinteger(Lua, 35);
	lua_setglobal(Lua, "BLOCK_GRAY");
	lua_pushinteger(Lua, 36);
	lua_setglobal(Lua, "BLOCK_WHITE");
	lua_pushinteger(Lua, 37);
	lua_setglobal(Lua, "BLOCK_YFLOWER");
	lua_pushinteger(Lua, 38);
	lua_setglobal(Lua, "BLOCK_RFLOWER");
	lua_pushinteger(Lua, 39);
	lua_setglobal(Lua, "BLOCK_BSHROOM");
	lua_pushinteger(Lua, 40);
	lua_setglobal(Lua, "BLOCK_RSHROOM");
	lua_pushinteger(Lua, 41);
	lua_setglobal(Lua, "BLOCK_GOLD");
	lua_pushinteger(Lua, 42);
	lua_setglobal(Lua, "BLOCK_IRON");
	lua_pushinteger(Lua, 43);
	lua_setglobal(Lua, "BLOCK_DSTAIR");
	lua_pushinteger(Lua, 43);
	lua_setglobal(Lua, "BLOCK_DOUBLESTAIR");
	lua_pushinteger(Lua, 43);
	lua_setglobal(Lua, "BLOCK_DSTEP");
	lua_pushinteger(Lua, 43);
	lua_setglobal(Lua, "BLOCK_DOUBLESTEP");
	lua_pushinteger(Lua, 44);
	lua_setglobal(Lua, "BLOCK_STAIR");
	lua_pushinteger(Lua, 44);
	lua_setglobal(Lua, "BLOCK_STEP");
	lua_pushinteger(Lua, 45);
	lua_setglobal(Lua, "BLOCK_BRICK");
	lua_pushinteger(Lua, 46);
	lua_setglobal(Lua, "BLOCK_TNT");
	lua_pushinteger(Lua, 47);
	lua_setglobal(Lua, "BLOCK_BOOK");
	lua_pushinteger(Lua, 47);
	lua_setglobal(Lua, "BLOCK_BOOKS");
	lua_pushinteger(Lua, 47);
	lua_setglobal(Lua, "BLOCK_BOOKCASE");
	lua_pushinteger(Lua, 48);
	lua_setglobal(Lua, "BLOCK_MOSSCOB");
	lua_pushinteger(Lua, 48);
	lua_setglobal(Lua, "BLOCK_MOSSCOBBLE");
	lua_pushinteger(Lua, 48);
	lua_setglobal(Lua, "BLOCK_MCOBBLE");
	lua_pushinteger(Lua, 49);
	lua_setglobal(Lua, "BLOCK_OBSIDIAN");
	lua_pushinteger(Lua, 49);
	lua_setglobal(Lua, "BLOCK_BLACKROCK");
	lua_pushinteger(Lua, 128);
	lua_setglobal(Lua, "BLOCK_INVALID");
}

void PluginManager::SetTimeout(int seconds, const char *_function, Plugin *_plugin) {
	Timeout *newTimeout = new Timeout();
	newTimeout->function = (char*)malloc(strlength(_function)+1);
	strcpy(newTimeout->function, _function);
	time_t t;
	t = time(0);
	newTimeout->time = t + seconds;
	newTimeout->plugin = _plugin;
	newTimeout->next = NULL;
	
	if(this->timeouts_front && this->timeouts_back) {
		this->timeouts_back->next = newTimeout;
		this->timeouts_back = newTimeout;
	} else {
		this->timeouts_front = newTimeout;
		this->timeouts_back = newTimeout;
	}
}
	
void PluginManager::RemoveTimeout(Timeout *_timeout) {
	Timeout *temp;
	if(this->timeouts_front == _timeout) {
		if(this->timeouts_front->next) {
			temp = this->timeouts_front;
			this->timeouts_front = temp->next;
			delete temp;
		} else {
			delete this->timeouts_front;
			this->timeouts_front = NULL;
			this->timeouts_back = NULL;
		}
	} else {
		for(Timeout *t = this->timeouts_front; t;) {
			if(t->next == _timeout) {
				temp = t->next;
				if(temp == this->timeouts_back)
					this->timeouts_back = t;
				t->next = temp->next;
				delete temp;
				break;
			} else
				t = t->next;
		}
	}
}

Zone *PluginManager::CreateZone(Level *_level, block _block1, block _block2) {
	Zone *newZone = new Zone(_level, _block1, _block2, this->server);
	
	if(this->zones_back && this->zones_front) {
		this->zones_back->next = newZone;
		this->zones_back = newZone;
	} else {
		this->zones_front = newZone;
		this->zones_back = newZone;
	}

	return newZone;
}

bool PluginManager::RemoveZone(Zone *zone) {
	Zone *z;
	Zone *target;
	for(z = this->zones_front; z; z = z->next) {
		if(z->next == zone) {
			target = z->next;
			if(target == this->zones_back)
				this->zones_back = z;
			z->next = target->next;
			delete target;
			return true;
		}
	}
	if(this->zones_front == zone) {
		this->zones_front = zone->next;
		if(this->zones_back == zone)
			this->zones_back = NULL;
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

void Plugin::AddChatCommand(const char *_command, const char *_function) {
	this->plugins->AddChatCommand(_command, _function, this);
}

void Plugin::SetTimeout(int seconds, const char *_function) {
	this->plugins->SetTimeout(seconds, _function, this);
}

void Plugin::SetName(const char *_name) {
	this->name = (char*)malloc(strlength(_name)+1);
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

int PluginManager::GetPlayers(lua_State *Lua) {
	lua_getfield(Lua, LUA_GLOBALSINDEX, "SERVER");
	Server *server;
	server = (Server*)tolua_tousertype(Lua, -1, 0);
	lua_pop(Lua, 1);

	std::vector<Player*> localVec = server->GetPlayers();

	int num = 0;
	lua_newtable(Lua);

	for(int i=0; i<localVec.size(); ++i) {
		num++;
		lua_pushnumber(Lua, num);
		tolua_pushusertype(Lua, (void*)localVec.at(i), "Player");
		lua_settable(Lua, -3);
	}

	return 1;
}

int PluginManager::GetPlayersInWorld(lua_State *Lua) {
	lua_getfield(Lua, LUA_GLOBALSINDEX, "SERVER");
	Server *server;
	server = (Server*)tolua_tousertype(Lua, -1, 0);
	lua_pop(Lua, 1);

	char *world;
	world = (char*)lua_tostring(Lua, -1);
	lua_pop(Lua, -1);

	std::vector<Player*> localVec = server->GetPlayers();

	int num = 0;
	lua_newtable(Lua);

	for(int i=0; i<localVec.size(); ++i) {
		if(strcmp(localVec.at(i)->GetWorld(), world))
			continue;
		num++;
		lua_pushnumber(Lua, num);
		tolua_pushusertype(Lua, (void*)localVec.at(i), "Player");
		lua_settable(Lua, -3);
	}

	return 1;
}

int PluginManager::GetBlocksOfType(lua_State *Lua) {
	lua_getfield(Lua, LUA_GLOBALSINDEX, "SERVER");
	Server *server;
	server = (Server*)tolua_tousertype(Lua, -1, 0);
	lua_pop(Lua, 1);

	int type;
	type = lua_tointeger(Lua, -1);
	lua_pop(Lua, 1);

	char *world;
	world = (char*)lua_tostring(Lua, -1);
	lua_pop(Lua, 1);

	if(!server->GetLevelByName(world)) {
		lua_pushnil(Lua);
		return 1;
	}

	//std::vector<block> *localVec;
	//localVec = server->GetBlocksOfType(world, (char)type);

	Level *lvl;
	lvl = server->GetLevelByName(world);
	if(!lvl)
		return NULL;
	int dimX = lvl->getDimX();
	int dimY = lvl->getDimY();
	int dimZ = lvl->getDimZ();

	int num = 0;
	lua_newtable(Lua);

	for(int x=0; x<dimX; ++x) {
		for(int y=0; y<dimY; ++y) {
			for(int z=0; z<dimZ; ++z) {
				block b;
				b.blocktype= lvl->getBlock(x, y, z);
				if(b.blocktype != type)
					continue;
				num++;
				lua_pushnumber(Lua, num);
				tolua_pushusertype(Lua, (void*)&b, "block");
				lua_settable(Lua, -3);
			}
		}
	}

	return 1;
}