--Author : Dandistine
--
--Plugin : *nix Style Manual function, abbreviated 'man'
--
--Purpose: Provides a unified help and manual method for the LuaCraft server
--         to allow easier development of additional plugins.  This method is
--         intended to make lives easier for developers and players alike by
--         creating a standard help function
--
--Useage : To use Man, add this to your plugins fodler and plugins.txt.  Other 
--		   plugins can, and should, use the AddMan function to add entries to
--		   the Manual indexing table.
--Example: no examples yet using the new plugin system
--	      
--
--
--
--Future : In the future (when i learn more lua) I will be cleaning up this code a 
--         great deal.  At the moment it works, most likely a bit inefficient.

GlobalManIndex = {};
NumberOfLines = {};

SupressConsole = 1;

PLUGIN:SetName("Manual");

function PrintMan(player, command)
	command = string.lower(command);
	if NumberOfLines[command] == nil then
		player:SendMessage("Man: Command manual not found for the "..command.." command!");
	else
		for i = 1, NumberOfLines[command], 1 do
			player:SendMessage("Man: "..GlobalManIndex[command..i]);
		end
	end
end
	
function AddMan(command, description)
	--should only be called on initilization
	--or by other plugins, do not call from a chat command
	if command == nil then
		error("Cannot add a Nil command.");
		command = string.lower(command);
	end
	if SupressConsole == 0 then
		print("Adding "..command.." to Manual table.");
	end
	if NumberOfLines[command] == nil then
		NumberOfLines[command] = 1;
	else
		NumberOfLines[command] = NumberOfLines[command] + 1;
	end
	GlobalManIndex[command..NumberOfLines[command]] = description;
end

function RemoveMan(player, command)
	--"could" be called in-game
	--not suggested though
	command = string.lower(command);
	if NumberOfLines[command] == nil then
		error("Command "..command.." not found!");
	else
		for i = 1, NumberOfLines[command], 1 do
			GlobalManIndex[command..i] = nil;
		end
		NumberOfLines[command] = nil;
		if player ~= nil then
			player:SendMessage("Man: Sucessfully removed "..command.." from the table.");
		end
	end
end

function HandleChat(player)
	--get player's chat message
	local chatmsg = player:GetLastChat();
	--no need to send to console, we only handle the command usage
	if chatmsg:sub(0,4) == "/man" then
		PrintMan(player, chatmsg:sub(6, string.len(chatmsg)));
		if SupressConsole == 0 then
			print(player:GetName().." used Man on "..chatmsg:sub(6, string.len(chatmsg)));
		end
	end
end
PLUGIN:AddHook("OnPlayerChat", "HandleChat");