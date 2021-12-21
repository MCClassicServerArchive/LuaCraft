-- Freeze command
function CmdFreeze(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /freeze <name>");
		return;
	end
	local target = SERVER:GetPlayerByName(param);
	if target == nil then
		player:SendMessage("&ePlayer not found!");
	else
		if target.IsFrozen then
			target.IsFrozen = false;
			target:SendMessage("&eYou were unfrozen by "..player:GetName());
			player:SendMessage("&eYou unfroze "..target:GetName());
			print("Player ["..player:GetName().."] unfroze player ["..target:GetName().."]");
		else
			target.IsFrozen = true;
			target:SendMessage("&eYou were frozen by "..player:GetName());
			player:SendMessage("&eYou froze "..target:GetName());
			print("Player ["..player:GetName().."] froze player ["..target:GetName().."]");
		end
	end
end
GAMEMODE:AddChatCommand("freeze", "CmdFreeze");