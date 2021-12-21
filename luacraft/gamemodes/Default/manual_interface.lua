-- Add man entries to Dandistine's "Manual" plugin
function AddMan(commandName, entryText)
	local ok, msg = pcall(Manual.AddMan, commandName, entryText);
	if not ok then error(msg) end
end

-- Once all plugins are loaded, do add the Man entries to Dandistine's "Manual" plugin
function OnPluginsLoaded()
	-- Find Dandistine's "Manual" plugin
	Manual = SERVER:GetPluginByName("Manual");
	if Manual == nil then
		print("Manual plugin not loaded. No entries added.")
		return;
	else
		print("Manual plugin found. Adding entries.")
	end
	
	AddMan("freeze", "Use /freeze <name> to freeze a player [OP Only]");
	AddMan("me", "Use /me <message> to say an action");
	AddMan("say", "Use /say <message> to say a message w/o a name");
	AddMan("deop", "Use /deop <name> to make a player not an OP [OP Only]");
	AddMan("op", "Use /op <name> to make a player an OP [OP Only]");
	AddMan("tp", "Use /tp <name> to teleport to that player [OP Only]");
	AddMan("bring", "Use /bring <name> to teleport that player to you [OP Only]");
	AddMan("spec", "Use /spec <name> to watch player in first person [OP Only]");
	AddMan("kick", "Use /kick <name> to kick that player [OP Only]");
	AddMan("ban", "Use /ban <name> to ban that player [OP Only]");
	AddMan("paint", "Use /paint to toggle painting status.");
	AddMan("dense", "Use /dense to toggle placing dense blocks.");
	AddMan("forcesave", "Use /forcesave to save the map file.");
	AddMan("new", "Use /new <world_name> to create a new world. [OP Only]");
	AddMan("join", "Use /join <world_name> to join a world.");
	AddMan("worlds", "Use /worlds to list the current worlds.");
	AddMan("players", "Use /players to list all players in the server.");
	AddMan("players", "Use /players <world_name> to list all players in world.");
	AddMan("set", "Use /set <block_type> to make stone become that block.");
	AddMan("set", "Types of blocks:");
	AddMan("set", "  &2water");
	AddMan("set", "  &2lava");
	AddMan("set", "  &2air");
	AddMan("set", "  &2grass");
	AddMan("set", "  &2opcrete &e[OP Only]");
end
GAMEMODE:AddHook("OnPluginsLoaded", "OnPluginsLoaded");