-- On player chat
function HandleChat(player)	
	-- Print their message to the console
	print(player:GetName()..": "..player:GetLastChat());
end
GAMEMODE:AddHook("OnPlayerChat", "HandleChat");

-- When player chat should be broadcasted (No plugins decided it was a command)
function BroadcastChat(player)
	-- If it wasn't supressed but starts with a '/', treat it as an unknown command
	if player:GetLastChat():sub(0,1) == "/" then
		player:SendMessage("&cUnknown command: &f"..split(player:GetLastChat():sub(2), " ")[1]);
		return;
	end
	
	-- Send the chat message to all players in server
	if player:GetOP() then
		SERVER:SendChat("&2["..player:GetWorld().."] "..player:GetDisplayName().."&f: "..player:GetLastChat():gsub("_","&"));
	else
		SERVER:SendChat("&2["..player:GetWorld().."] "..player:GetDisplayName().."&f: "..player:GetLastChat());
	end
end
GAMEMODE:AddHook("PlayerShouldChat", "BroadcastChat");

-- Say command
function CmdSay(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		print("invalid say!");
		player:SendMessage("&eInvalid format! Use /say <message>");
		return;
	end
	if param:sub(0,1) == "_" then
		SERVER:SendChat(param:gsub("_","&"));
	else
		SERVER:SendChat("&e"..param:gsub("_","&"));
	end
end
GAMEMODE:AddChatCommand("say", "CmdSay");

-- Me command
function CmdMe(player, param)
	if not param then
		player:SendMessage("&eInvalid format! Use /me <message>");
		return;
	end
	if player:GetOP() then param = param:gsub("_","&") end
	SERVER:SendChat("&d* "..player:GetName().." "..param);
end
GAMEMODE:AddChatCommand("me", "CmdMe");