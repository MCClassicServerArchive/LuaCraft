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