PLUGIN:SetName("RememberMod");
local rememberText = "No Message";

-- On player chat
function HandleChat(player)		
	-- Get what they said
	local chatMsg = player:GetLastChat();
	
	-- Parse text for command
	if chatMsg == "/remember" then
		player:SendMessage("&e"..rememberText);
	elseif chatMsg:sub(1,10) == "/remember " then
		rememberText = chatMsg:sub(11);
		player:SendMessage("&9Saved:&f "..rememberText:sub(1,-1));
	end
	
end
PLUGIN:AddHook("OnPlayerChat", "HandleChat");

-- Add manual entry
function PluginsLoaded()
	if GetPluginByName("Manual") then
		local ok, msg = pcall(GetPluginByName("Manual").AddMan, "remember", "Use /remember to see the last \"remembered\" message");
		local ok, msg = pcall(GetPluginByName("Manual").AddMan, "remember", "Use /remember <message> to \"remember\" a message");
		if not ok then error(msg) end
	end
end
PLUGIN:AddHook("OnPluginsLoaded", "PluginsLoaded");