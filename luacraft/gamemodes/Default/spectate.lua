-- Spectate command
function CmdSpectate(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		if player.IsSpectating then
			player.IsSpectating = false;
			player:SendMessage("&eYou are no longer spectating "..SERVER:GetPlayerByName(player.SpectatingTarget):GetDisplayName());
			player:SetPos(player.PreSpecPos);
		else
			player:SendMessage("&eInvalid format! Use /spec <name>");
		end
	else
		local target = SERVER:GetPlayerByName(param);
		if target == nil then
			player:SendMessage("&ePlayer not found!");
		else
			if player.IsSpectating then
				player.IsSpectating = false;
				player:SendMessage("&eYou are no longer spectating "..SERVER:GetPlayerByName(player.SpectatingTarget):GetDisplayname());
				player:SetPos(player.PreSpecPos);
			end
			player.IsSpectating = true;
			player.SpectatingTarget = target:GetName();
			player.PreSpecPos = player:GetPos();
			print("Player ["..player:GetName().."] is now spectating player ["..target:GetName().."]");
			player:SendMessage("&eYou are now spectating "..target:GetDisplayName());
		end
	end
end
GAMEMODE:AddChatCommand("spectate", "CmdSpectate");
GAMEMODE:AddChatCommand("spec", "CmdSpectate");