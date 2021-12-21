-----------------------------------------
-- Title: Labels LuaCraft plugin
-- Description: Allow OPs to place custom messages on map
-- Author: TheJCS (mail@thejcs.com.br)
-- Version: in-dev
-----------------------------------------

PLUGIN:SetName("Labels");

-----------------------------------------
-- Helper functions
-----------------------------------------

-- Get the integer part of the number
function truncate(n)
	if n < 0 then
		return math.ceil(n);
	else
		return math.floor(n);
	end
end

-- Split a string on "div"
function string:explode(div)
	if (div=='') then
		return self;
	end
	
	local pos = 0;
	local tab = {};
	
	-- for each divider found
	for st,sp in function() return string.find(self,div,pos,true); end do
		table.insert(tab,string.sub(self,pos,st-1)); -- Attach chars left of current divider
		pos = sp + 1; -- Jump past current divider
	end
	
	table.insert(tab,string.sub(self,pos)); -- Attach chars right of last divider
	
	return tab;
end

-----------------------------------------
-- Global declarations
-----------------------------------------

labels = {};

-----------------------------------------
-- Main functions
-----------------------------------------

function init()
	-- Check for the labels file and create one if needed
	local labelsFile = io.open("data/labels.txt", "r");
	if labelsFile == nil then
		print("Could not find data/labels.txt!");
		saveLabels(nil);
		print("Default labels file created. Read it for instructions");
	else
		labelsFile:close();
	end
	
	loadLabels();
end

function loadLabels()
	local file = io.open("data/labels.txt", "r");
	if(file == nil) then
		print("Could not read the labels file!");
		return;
	end
	
	local inLabel = nil;
	local lineNum = 1;
	local numLabels = 0;
	
	labels = {};
	
	for line in file:lines() do
		-- Lines starting with # are comments, so ignore.
		if line:sub(0,1) ~= "#" then
			if inLabel == nil then
				-- Empty lines aren't read
				if line ~= "" then					
					inLabel = line;
					labels[line] = {};
				end
			else
				if line == "" then
					
					-- An empty line marks the end of the label
					inLabel = nil;
					numLabels = numLabels + 1;
				else
					-- Feed our label data 
					table.insert(labels[inLabel], line);
				end
			end
		end
		
		lineNum = lineNum + 1;
	end
	
	-- LUA appears to don't read blank lines at the end of the file, so we need this
	if inLabel ~= nil then
		numLabels = numLabels + 1;
	end
	
	print("Loaded "..numLabels.." label(s) from file");
	
	file:close();
end

function saveLabels()
	-- Open file
	local file = io.open("data/labels.txt", "w+");
	if(file == nil) then
		print("Could not read the labels file!");
		return;
	end
	
	-- Create the default header
	file:write("# Lines starting with '#' are comments, like this one\n");
	file:write("# \n");
	file:write("# You can create a label in game, with /label or directly here\n");
	file:write("# \n");
	file:write("# Each label starts with the x,y,z coordinators and the world on the first line\n");
	file:write("# Each line after it will be displayed when the player moves on that point\n");
	file:write("# A blank line mark the end of that label\n");
	file:write("# \n");
	file:write("# Color codes may be used\n");
	file:write("\n");
		
	-- If we have labels to write, do it
	for labelId, label in pairs(labels) do
		-- The first line of the label is the x,y,z coordinates of it
		file:write(labelId.."\n");
		
		-- Write each line of the label
		for lineId, line in pairs(label) do
			file:write(line.."\n");
		end
		
		-- The blank line is the end of the label
		file:write("\n");
	end
	
	file:close();
end

function HandleJoin(player)
	-- When join, zero our props
	player.writingLabel = nil;
	player.labelId = nil;
end

function HandleChat(player)		
	local chatMsg = player:GetLastChat():gsub("_","&");
	
	-- Check for the /label command, which starts or end a label
	if chatMsg == "/label" then
		-- Only OPs can use it
		if player:GetOP() then
			-- Our player will begin a label
			if player.writingLabel == nil then
				-- Make the labelId
				local labelId = player.labelId;
				
				-- Store the labelId
				player.writingLabel = labelId;
				
				-- If there's a label there, he will edit it
				if labels[labelId] == nil then
					player:SendMessage("Creating label for "..labelId);
				else
					player:SendMessage("Editing label for "..labelId);
				end
				
				-- Zero the label messages
				labels[labelId] = {};
				
				player:SendMessage("Each message you send from now will be added to the label");
				player:SendMessage("Send /label again to stop");
				player:SendMessage("If you don't send any message, this label will be removed");
			else
				-- Nothing was sent, remove the label
				if table.maxn(labels[player.writingLabel]) == 0 then
					labels[player.writingLabel] = nil;
					
					player:SendMessage("Removed label");
				else
					player:SendMessage("Done labeling");
				end
				
				player.writingLabel = nil;
				
				-- Store the labels on the file..
				saveLabels();
			end
		else
			player:SendMessage("You are not allowed to use this command");
		end
		player:SupressChat();
	-- If we writing on a label, it grabs the message
	elseif player.writingLabel ~= nil then
		table.insert(labels[player.writingLabel], chatMsg);
		player:SupressChat();
	end
end

--Movement hook for the game
function HandleMovement(player)
	local pos = player:GetPos();
	local current = truncate(pos.x)..","..truncate(pos.y)..","..truncate(pos.z)..","..player:GetWorld();
	
	if current ~= player.labelId then
		if labels[current] ~= nil then
			for lineId, line in pairs(labels[current]) do
				player:SendMessage(line);
			end
		end
		
		player.labelId = current;
	end
end

-----------------------------------------
-- Function calls
-----------------------------------------

init();

PLUGIN:AddHook("OnPlayerMove", "HandleMovement");
PLUGIN:AddHook("OnPlayerJoin", "HandleJoin");
PLUGIN:AddHook("OnPlayerChat", "HandleChat");