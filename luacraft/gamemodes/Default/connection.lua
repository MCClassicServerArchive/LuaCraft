-- On player connect
function HandleConnect(player)
	-- If the player is banned, kick their butt outta here
	if IsBanned(player) then
		player:Kick("You are banned!");
		return;
	end

	-- If the player is dag10, make them an OP (for testing purposes)
	if player:GetName() == "dag10" then
		player:SetOP(true);
	end
	
	-- If they're an OP, make their name color for chat different
	if player:GetOP() then
		player.Color = "&9";
	else
		player.Color = "&f";
	end
	
	-- Spawn them in the main world
	if player:SetWorld("main") == 0 then
		print("WARNING: Failed to set world \"main\" for ["..player:GetName().."]");
		player:Kick("Invalid World!");
		return;
	end
	
	-- Set their in-game name to be colored
	player:SetDisplayName(player.Color..player:GetName());
	
	-- Print that they joined to the server console
	print("Player ["..player:GetName().."] has joined the game. ["..player:GetIP().."]");
	
	-- Tell all players in their world that they joined
	SERVER:SendChat(player:GetDisplayName().."&e has joined the game!")
	
	-- Send a welome message to them
	player:SendMessage("&aWelcome to &eLuaCraft&a, "..player:GetName().."!");
end
GAMEMODE:AddHook("OnPlayerJoin", "HandleConnect");


-- On player disconnect
function HandleDisconnect(player)
	-- Print that they left to the server console
	print("Player ["..player:GetName().."] has left the game. ["..player:GetIP().."]");
	
	-- Tell all players in their world that they left
	SERVER:SendChat(player:GetDisplayName().."&e has left the game!")
end
GAMEMODE:AddHook("OnPlayerLeave", "HandleDisconnect");