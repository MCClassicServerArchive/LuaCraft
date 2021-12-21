-- Teleport command
function CmdTeleport(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /tp <name>");
		return;
	end
	local target = SERVER:GetPlayerByName(param);
	if target == nil then
		player:SendMessage("&ePlayer not found!");
	else
		player:SendToPlayer(target);
		print("Player ["..player:GetName().."] teleported to player ["..target:GetName().."]");
	end
end
GAMEMODE:AddChatCommand("tp", "CmdTeleport");
		
-- Bring command
function CmdBring(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /bring <name>");
		return;
	end
	local target = SERVER:GetPlayerByName(param);
	if target == nil then
		player:SendMessage("&ePlayer not found!");
	else
		target:SendToPlayer(player);
		print("Player ["..player:GetName().."] brought player ["..target:GetName().."] to them");
		target:SendMessage(player:GetDisplayName().." &ebrought you to them!");
	end
end
GAMEMODE:AddChatCommand("bring", "CmdBring");