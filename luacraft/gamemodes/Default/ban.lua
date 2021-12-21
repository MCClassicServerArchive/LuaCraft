-- Create bans list if not existant
local BanList = io.open("cfg/bans.txt", "a+");
if not BanList then
	BanList = io.open("cfg/bans.txt", "w");
	BanList:write("");
	BanList:close();
end

-- Check if player is banned
function IsBanned(player)
	local BanList = io.open("cfg/bans.txt");
	if not BanList then
		BanList = io.open("cfg/bans.txt", "w");
		BanList:write("");
		BanList:close();
		return false;
	else
		for line in BanList:lines() do
			lineParts = split(line, " ");
			if (lineParts[1] == player:GetName()) or (lineParts[2] == player:GetIP()) then
				BanList:close();
				return true;
			end
		end
	end
	return false;
end

-- Ban player
function Ban(player, msg)
	local BanList = io.open("cfg/bans.txt", "a+");
	if not BanList then
		BanList = io.open("cfg/bans.txt", "w");
		BanList:write(player:GetName().." "..player:GetIP().."\n");
		BanList:close();
	else
		BanList:seek("end");
		BanList:write(player:GetName().." "..player:GetIP().."\n");
		BanList:close();
		player.WasKicked = true;
		SERVER:SendChat(player:GetDisplayName().."&e has been banned.");
		player:Kick(msg);
	end
end

-- Unban player by name
function Unban(name)
	local BanList = io.open("cfg/bans.txt");
	if not BanList then
		BanList = io.open("cfg/bans.txt", "w");
		BanList:write("");
		BanList:close();
		return false;
	end
	local OldLines = {};
	local k = 1;
	for line in BanList:lines() do
		OldLines[k] = line;
		k = k + 1;
	end
	BanList:close();
	BanList = io.open("cfg/bans.txt", "w+");
	local FoundEntry = false;
	for k, line in pairs(OldLines) do
		local lineParts = split(line, " ");
		if lineParts[1] == name then
			FoundEntry = true;
		else
			BanList:write(line.."\n");
		end
	end
	BanList:close();
	return FoundEntry;
end

-- Unban player by IP
function UnbanIP(ip)
	local BanList = io.open("cfg/bans.txt");
	if not BanList then
		BanList = io.open("cfg/bans.txt", "w");
		BanList:write("");
		BanList:close();
		return false;
	end
	local OldLines = {};
	local k = 1;
	for line in BanList:lines() do
		OldLines[k] = line;
		k = k + 1;
	end
	BanList:close();
	BanList = io.open("cfg/bans.txt", "w+");
	local FoundEntry = false;
	for k, line in pairs(OldLines) do
		local lineParts = split(line, " ");
		if lineParts[2] == ip then
			FoundEntry = true;
		else
			BanList:write(line.."\n");
		end
	end
	BanList:close();
	return FoundEntry;
end

-- Kick command
function CmdKick(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /kick <name>");
		return;
	end
	local target = SERVER:GetPlayerByName(param);
	if target == nil then
		player:SendMessage("&ePlayer not found!");
	else
		player.WasKicked = true;
		SERVER:SendChat(player:GetDisplayName().."&e was kicked by "..player:GetDisplayName());
		target:Kick("You were kicked by "..player:GetDisplayName());
	end
end
GAMEMODE:AddChatCommand("kick", "CmdKick");		
	
-- Ban command
function CmdBan(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /ban <name>");
		return;
	end
	local target = SERVER:GetPlayerByName(param);
	if target == nil then
		player:SendMessage("&ePlayer not found!");
	else
		Ban(target, "You were banned by "..player:GetDisplayName());
	end
end
GAMEMODE:AddChatCommand("ban", "CmdBan");

-- Unban command
function CmdUnban(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /unban <name>");
		return;
	end
	if Unban(param) then
		player:SendMessage("&eSuccessfully unbanned &2"..param);
	else
		player:SendMessage("&eCouldn't find &2"..param.." &ein cfg/bans.txt");
	end
end
GAMEMODE:AddChatCommand("unban", "CmdUnban");
		
-- UnbanIP command
function CmdUnbanIP(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /unbanip <ip>");
		return;
	end
	if UnbanIP(param) then
		player:SendMessage("&eSuccessfully unbanned IP &2"..param);
	else
		player:SendMessage("&eCouldn't find IP &2"..param.." &ein cfg/bans.txt");
	end
end
GAMEMODE:AddChatCommand("unbanip", "CmdUnbanIP");