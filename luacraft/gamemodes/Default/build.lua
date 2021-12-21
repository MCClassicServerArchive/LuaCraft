-- Dense command
function CmdDense(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if player.IsDense then
		player.IsDense = false;
		player:SendMessage("&eYou are no longer placing dense blocks.");
	else
		player.IsDense = true;
		player:SendMessage("&eYou are now placing dense blocks");
	end
end
GAMEMODE:AddChatCommand("dense", "CmdDense");
	
-- Paint command
function CmdPaint(player, param)
	if player.IsPainting then
		player.IsPainting = false;
		player:SendMessage("&eYou are no longer painting.");
	else
		player.IsPainting = true;
		player:SendMessage("&eYou are now painting.");
	end
end
GAMEMODE:AddChatCommand("paint", "CmdPaint");
GAMEMODE:AddChatCommand("p", "CmdPaint");

-- Set block command
function CmdSetBlock(player, param)
	if param == nil or param == "stone" or param == "s" then
		player.CurBlock = nil;
	elseif param == "water" or param == "w" then
		player.CurBlock = "water";
	elseif param == "air" or param == "a" then
		player.CurBlock = "air";
	elseif param == "grass" or param == "g" then
		player.CurBlock = "grass";
	elseif param == "lava" or param == "l" then
		player.CurBlock = "lava";
	elseif param == "opcrete" or param == "o" then
		if player:GetOP() then
			player.CurBlock = "opcrete";
		else
			player:SendMessage("&cOnly OPs can place opcrete!");
			return;
		end
	else
		player:SendMessage("&eUnknown block type: "..param);
		return;
	end
	if param then
		player:SendMessage("&eYou are now placing &2"..player.CurBlock);
	else
		player:SendMessage("&eYou are now placing &2normal stone");
	end
end
GAMEMODE:AddChatCommand("set", "CmdSetBlock");
GAMEMODE:AddChatCommand("s", "CmdSetBlock");