-- On player connect
function HandleConnect(player)
	-- If the player is banned, kick their butt outta here
	if IsBanned(player) then
		player.WasKicked = true;
		SERVER:SendChat(player:GetDisplayName().."&e has left the server!");
		SERVER:SendChat(player:GetDisplayName().."&e has been kicked: You are banned!");
		player:Kick("You are banned!");
		return;
	end

	-- If the player is in /cfg/ops.txt, OP them
	if IsInOpList(player) then
		player:SetOP(true);
		player.Color = "&9";
	else
		player.Color = "&f";
	end
	
	-- Spawn them in the main world
	if player:SetWorld(DefaultWorld) == 0 then
		print("WARNING: Failed to set world \""..DefaultWorld.."\" for ["..player:GetName().."]");
		player.WasKicked = true;
		SERVER:SendChat(player:GetDisplayName().."&e has left the server!");
		SERVER:SendChat(player:GetDisplayName().."&e has been kicked: Invalid World!");
		player:Kick("Invalid World!");
		return;
	end
	
	-- Set their in-game name to be colored
	player:SetDisplayName(player.Color..player:GetName());
	
	-- Print that they joined to the server console
	print("Player ["..player:GetName().."] has joined the server. ["..player:GetIP().."]");
	
	-- Tell all players in their world that they joined
	SERVER:SendChat(player:GetDisplayName().."&e has joined the server!")
	
	-- Send a welome message to them
	player:SendMessage("&aWelcome to &eLuaCraft&a, "..player:GetName().."!");
end
GAMEMODE:AddHook("OnPlayerJoin", "HandleConnect");


-- On player disconnect
function HandleDisconnect(player)
	-- Print that they left to the server console
	print("Player ["..player:GetName().."] has left the server. ["..player:GetIP().."]");
	
	-- Don't send this message to in-game players
	if player.WasKicked then return end;
	
	-- Tell all players in their world that they left
	SERVER:SendChat(player:GetDisplayName().."&e has left the server!");
end
GAMEMODE:AddHook("OnPlayerLeave", "HandleDisconnect");