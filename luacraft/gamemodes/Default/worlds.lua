-- Load worlds from worlds.txt
local WorldsList = io.open("data/worlds.txt");
if not WorldsList then
	WorldsList = io.open("data/worlds.txt", "w");
	WorldsList:write("main main.dat\n");
	SERVER:CreateWorld("main", "main.dat");
	DefaultWorld = "main";
	WorldsList:close();
else for line in WorldsList:lines() do
		lineParts = split(line, " ");
		SERVER:CreateWorld(lineParts[1], lineParts[2]);
		if not DefaultWorld then
			DefaultWorld = lineParts[1];
		end
	end
	WorldsList:close();
end

-- Save all worlds
function SaveWorlds()
	local wList = ListWorlds();
	for k, world in pairs(wList) do
		SERVER:GetLevelByName(world):Save();
	end
	print("Saved all worlds!");
end

-- List worlds
function ListWorlds()
	local retTable = {};
	local WorldsList = io.open("data/worlds.txt");
	if not WorldsList then
		return retTable;
	else for line in WorldsList:lines() do
			lineParts = split(line, " ");
			table.insert(retTable, lineParts[1]);
		end
		WorldsList:close();
	end
	return retTable;
end

-- Check if a world exists
function WorldExists(world)
	local WorldsList = io.open("data/worlds.txt");
	if not WorldsList then return false end
	for line in WorldsList:lines() do
		lineParts = split(line, " ");
		if lineParts[1]:lower() == world:lower() then return true end
	end
	WorldsList:close();
	return false;
end

-- Create a new world and add it to worlds.txt
function NewWorld(world)
	if WorldExists(world) then
		return false;
	end
	local WorldsList = io.open("data/worlds.txt", "a+");
	if not WorldsList then
		WorldsList = io.open("data/worlds.txt", "w");
		WorldsList:write(world.." "..world..".dat\n");
		WorldsList:close();
		SERVER:CreateWorld(world, world..".dat");
	else
		WorldsList:seek("end");
		WorldsList:write(world.." "..world..".dat\n");
		WorldsList:close();
		SERVER:CreateWorld(world, world..".dat");
	end
	return true;
end


-- New command (Makes new world)
function CmdNew(player, param)
	if not player:GetOP() then
		player:SendMessage("&cYou must be an OP!");
		return;
	end
	if not param then
		player:SendMessage("&eInvalid format! Use /new <world_name>");
		return;
	end
	if not NewWorld(param) then
		player:SendMessage("&eWorld &2"..param.." &ealready exists!");
	else
		player:SendMessage("&eCreated new world: &2"..param);
	end
end
GAMEMODE:AddChatCommand("new", "CmdNew");
		
-- Join command (Joins a world)
function CmdJoin(player, param)
	if not param then
		player:SendMessage("&eInvalid format! Use /join <world_name>");
		return;
	end
	local OldWorld = player:GetWorld();
	player:SetMOTD("&eLoading World: &2"..param);
	if not player:SetWorld(param) then
		print("Failed to set world \""..param.."\" for ["..player:GetName().."]");
		player:SendMessage("&eWorld does not exist: &4"..param);
		player:SendMessage("&eYou can use &2/new &eto make a new world");
		return;
	end
	print("Player ["..player:GetName().."] joined world ["..param.."]");
	SERVER:SendChat(player:GetDisplayName().."&e has left to go to world &2"..player:GetWorld(), OldWorld);
	SERVER:SendChat(player:GetDisplayName().."&e has joined the world!", player:GetWorld());
	player:SendMessage("&eYou are now in world: &2"..player:GetWorld());
end
GAMEMODE:AddChatCommand("join", "CmdJoin");
GAMEMODE:AddChatCommand("j", "CmdJoin");
		
-- Save command
function CmdForceSave(player, param)
	if player:GetOP() then
		SERVER:GetLevelByName(player:GetWorld()):Save();
		SERVER:SendChat("&eLevel saved by "..player:GetDisplayName().." &eas &2"..player:GetWorld()..".dat", player:GetWorld());
		print("Player ["..player:GetName().."] saved the map");
	else
		player:SendMessage("&cYou must be an OP!");
	end
end
GAMEMODE:AddChatCommand("forcesave", "CmdForceSave");
	
-- Worlds command
function CmdWorlds(player, param)
	local worldList = ListWorlds();
	player:SendMessage("&4Worlds:");
	for k, w in pairs(worldList) do
		if w == player:GetWorld() then
			player:SendMessage("&2  "..w.."  &b("..SERVER:NumPlayersInWorld(w)..")");
		else
			player:SendMessage("&e  "..w.."  &b("..SERVER:NumPlayersInWorld(w)..")");
		end
	end
end
GAMEMODE:AddChatCommand("worlds", "CmdWorlds");
		
-- Players command
function CmdPlayers(player, param)
	if not param then
		player:SendMessage("&ePlayers in &2entire server&e:");
		for i, ply in pairs(SERVER:GetPlayers()) do
			player:SendMessage("&f  "..ply:GetDisplayName());
		end
	else
		player:SendMessage("&ePlayers in &2"..param.."&e:");
		local players = SERVER:GetPlayersInWorld(param);
		for i, ply in pairs(SERVER:GetPlayersInWorld(param)) do
			player:SendMessage("&f  "..ply:GetDisplayName());
		end
	end
end
GAMEMODE:AddChatCommand("players", "CmdPlayers");

-- Autosave loop
function AutosaveWorlds()
	SaveWorlds();
	GAMEMODE:SetTimeout(30, "AutosaveWorlds");
end

-- Start the autosave loop
AutosaveWorlds();