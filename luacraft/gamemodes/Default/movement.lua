--Movement hook for the game
function HandleMovement(player)
	if player.IsSpectating then
		if SERVER:GetPlayerByName(player.SpectatingTarget) 
		   and SERVER:GetPlayerByName(player.SpectatingTarget):GetWorld() == player:GetWorld() then
			player:SendToPlayer(SERVER:GetPlayerByName(player.SpectatingTarget));
		else
			player.IsSpectating = false;
			player:SendMessage("&eYou are no longer spectating "..SERVER:GetPlayerByName(player:GetString("SpectatingTarget")):GetDisplayName());
			player:SetPos(player.PreSpecPos);
		end
	elseif player.IsFrozen then
		player:Teleport(player:GetPos());
	else
		player:SetPos(player:GetNewPos());
	end
end
GAMEMODE:AddHook("OnPlayerMove", "HandleMovement");



-- THIS COMMAND IS TEMPORARY - FOR TESTING PURPOSES ONLY
GAMEMODE:AddChatCommand("pos", "MyPos");
function MyPos(player, param)
	player:SendMessage("&eYour position:");
	player:SendMessage("&e  X:&f "..player:GetX());
	player:SendMessage("&e  Y:&f "..player:GetY());
	player:SendMessage("&e  Z:&f "..player:GetZ());
end