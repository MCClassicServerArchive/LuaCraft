PLUGIN:SetName("Shutdown");

-- On player chat
function HandleChat(player)		
	-- Get what they said
	local chatMsg = player:GetLastChat();
	
	-- Shutdown command
	if chatMsg == "/shutdown" then
		if player:GetOP() then
			if player.WantsShutdown then
				SERVER.Quit = true;
			else
				player:SendMessage("&4Are you sure you want to shut down the server?");
				player:SendMessage("&4Type &e/shutdown &4again to confirm.");
				player.WantsShutdown = true;
			end
		else
			player:SendMessage("&eYou must be an OP!");
		end
	end
	
end
PLUGIN:AddHook("OnPlayerChat", "HandleChat");

-- Add manual entry
function PluginsLoaded()
	if GetPluginByName("Manual") then
		local ok, msg = pcall(GetPluginByName("Manual").AddMan, "shutdown", "Use /shutdown to shut down the server");
		if not ok then error(msg) end
	end
end
PLUGIN:AddHook("OnPluginsLoaded", "PluginsLoaded");