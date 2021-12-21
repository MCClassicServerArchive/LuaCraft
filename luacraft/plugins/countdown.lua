PLUGIN:SetName("Countdown");
timeLeft = 0;

-- Do a count
function Count()
	SERVER:SendChat("Countdown: "..timeLeft);
	if timeLeft > 0 then
		PLUGIN:SetTimeout(1, "Count");
		timeLeft = timeLeft - 1;
	end
end

-- On player chat
function HandleChat(player)		
	-- Get what they said
	local chatMsg = player:GetLastChat();
	
	-- Countdown command
	if chatMsg == "/countdown" then
		timeLeft = 3;
		Count();
		player:SupressChat();
	end
	
end
PLUGIN:AddHook("OnPlayerChat", "HandleChat");

-- Add manual entry
function PluginsLoaded()
	if GetPluginByName("Manual") then
		local ok, msg = pcall(GetPluginByName("Manual").AddMan, "countdown", "Use /countdown to start a 3-sec countdown.");
		if not ok then error(msg) end
	end
end
PLUGIN:AddHook("OnPluginsLoaded", "PluginsLoaded");