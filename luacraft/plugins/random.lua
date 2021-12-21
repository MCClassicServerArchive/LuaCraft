PLUGIN:SetName("RandMod");

-- On player chat
function HandleChat(player)		
	-- Get what they said
	local chatMsg = player:GetLastChat();
	
	-- Rand command
	if chatMsg == "/rand" then
		player:SendMessage("A random number is: "..math.random());
	end
	
end
PLUGIN:AddHook("OnPlayerChat", "HandleChat");

-- Add manual entry
function PluginsLoaded()
	if GetPluginByName("Manual") then
		local ok, msg = pcall(GetPluginByName("Manual").AddMan, "rand", "Use /rand to get a random number between 0 and 1");
		if not ok then error(msg) end
	end
end
PLUGIN:AddHook("OnPluginsLoaded", "PluginsLoaded");