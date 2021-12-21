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
function Ban(player)
	local BanList = io.open("cfg/bans.txt", "a+");
	if not BanList then
		BanList = io.open("cfg/bans.txt", "w");
		BanList:write(player:GetName().." "..player:GetIP().."\n");
		BanList:close();
	else
		BanList:seek("end");
		BanList:write(player:GetName().." "..player:GetIP().."\n");
		BanList:close();
		player:Kick("You are banned!");
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
	local OldLines = BanList:lines();
	BanList:close();
	BanList = io.open("cfg/bans.txt", "w+");
	for line in OldLines do
		local lineParts = split(line, " ");
		if lineParts[1] != name then
			BanList:write(line.."\n");
		end
	end
	BanList:close();
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
	local OldLines = BanList:lines();
	BanList:close();
	BanList = io.open("cfg/bans.txt", "w+");
	for line in OldLines do
		local lineParts = split(line, " ");
		if lineParts[2] != ip then
			BanList:write(line.."\n");
		end
	end
	BanList:close();
end