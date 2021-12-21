-- Add man entries to Dandistine's "Manual" plugin
function AddMan(commandName, entryText)
	local ok, msg = pcall(Manual.AddMan, commandName, entryText);
	if not ok then error(msg) end
end

-- Once all plugins are loaded, do add the Man entries to Dandistine's "Manual" plugin
function OnPluginsLoaded()
	-- Find Dandistine's "Manual" plugin
	Manual = GetPluginByName("Manual");
	if Manual == nil then
		print("Manual plugin not loaded. No entries added.")
		return;
	else
		print("Manual plugin found. Adding entries.")
	end
	
	AddMan("freeze", "Use /freeze <name> to freeze a player [OP Only]");
	AddMan("me", "Use /me <message> to say an action");
	AddMan("say", "Use /say <message> to say a message w/o a name");
	AddMan("deop", "Use /deop <name> to make a player not an OP");
	AddMan("op", "Use /op <name> to make a player an OP");
	AddMan("tp", "Use /tp <name> to teleport to that player [OP Only]");
	AddMan("bring", "Use /bring <name> to teleport that player to you [OP Only]");
	AddMan("spec", "Use /spec <name> to watch player in first person [OP Only]");
	AddMan("kick", "Use /kick <name> to kick that player [OP Only]");
	AddMan("paint", "Use /paint to toggle painting status.");
	AddMan("dense", "Use /dense to toggle placing dense blocks.");
	AddMan("save", "Use /save to save the map file.");
	AddMan("setspawn", "Use /setspawn to set where new players will spawn.");
end
GAMEMODE:AddHook("OnPluginsLoaded", "OnPluginsLoaded");