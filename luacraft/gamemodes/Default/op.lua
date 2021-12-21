-- Create OP list if not existant
local OpList = io.open("cfg/ops.txt", "a+");
if not OpList then
	OpList = io.open("cfg/ops.txt", "w");
	OpList:write("");
	OpList:close();
end

-- Check if player is op
function IsInOpList(player)
	local OpList = io.open("cfg/ops.txt");
	if not OpList then
		OpList = io.open("cfg/ops.txt", "w");
		OpList:write("");
		OpList:close();
		return false;
	else
		for line in OpList:lines() do
			if line == player:GetName() then
				OpList:close();
				return true;
			end
		end
	end
	return false;
end

-- Add player to OP list
function AddToOpList(player)
	local OpList = io.open("cfg/ops.txt", "a+");
	if not OpList then
		OpList = io.open("cfg/ops.txt", "w");
		OpList:write(player:GetName().."\n");
		OpList:close();
	else
		OpList:seek("end");
		OpList:write(player:GetName().."\n");
		OpList:close();
	end
end

-- Remove player from OP list
function RemoveFromOpList(name)
	local OpList = io.open("cfg/ops.txt");
	if not OpList then
		OpList = io.open("cfg/ops.txt", "w");
		OpList:write("");
		OpList:close();
		return false;
	end
	local OldLines = {};
	local k = 1;
	for line in OpList:lines() do
		OldLines[k] = line;
		k = k + 1;
	end
	OpList:close();
	OpList = io.open("cfg/ops.txt", "w+");
	local FoundEntry = false;
	for k, line in pairs(OldLines) do
		if line == name then
			FoundEntry = true;
		else
			OpList:write(line.."\n");
		end
	end
	OpList:close();
	return FoundEntry;
end

-- Op command
function CmdOp(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /op <name>");
		return;
	end
	local target = SERVER:GetPlayerByName(param);
	if target == nil then
		player:SendMessage("&ePlayer not found!");
	else
		if target:GetOP() then
			player:SendMessage("&ePlayer is already an OP!");
		else
			target:SetOP(true);
			AddToOpList(target);
			target.Color = "&9";
			target:SendMessage("&eYou are now an OP!");
			player:SendMessage("&eYou have just made "..target:GetName().." an OP!");
			print("Player ["..player:GetName().."] made player ["..target:GetName().."] an OP");
			
			-- Set their in-game name to be colored
			target:SetDisplayName(target.Color..target:GetName());
		end
	end
end
GAMEMODE:AddChatCommand("op", "CmdOp");
	
-- Deop command
function CmdDeop(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /deop <name>");
		return;
	end
	local target = SERVER:GetPlayerByName(param);
	if target == nil then
		player:SendMessage("&ePlayer not found!");
	else
		if target:GetOP() then
			target:SetOP(false);
			RemoveFromOpList(target:GetName());
			target.Color = "&f";
			target.IsDense = false;
			if target.CurBlock == BLOCK_OPCRETE then target.CurBlock = nil end
			target:SendMessage("&eYou are no longer an OP!");
			player:SendMessage("&eYou have just removed "..target:GetName().." as an OP!");
			print("Player ["..player:GetName().."] removed player ["..target:GetName().."] as an OP");
			
			-- Set their in-game name to be colored
			target:SetDisplayName(target.Color..target:GetName());
		else
			player:SendMessage("&ePlayer not an OP!");
		end
	end
end
GAMEMODE:AddChatCommand("deop", "CmdDeop");