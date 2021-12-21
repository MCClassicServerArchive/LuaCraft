PLUGIN:SetName("Rules");

-- Create default rules file if it doesn't exist
local rules = io.open("data/rules.txt", "r");
if rules == nil then
	print("Could not find data/rules.txt!");
	local newRules = assert(io.open("data/rules.txt", "w"));
	newRules:write("&cServer Rules:\n&ePlease type some rules here!\n&eThis file is in data/rules.txt");
	newRules:close();
	print("Created default rules file");
else
	rules:close();
end

-- Handle player connect
function HandleJoin(player)
	player:SendMessage("&aSay &4/rules &ato read our rules");
end
PLUGIN:AddHook("OnPlayerJoin", "HandleJoin");

-- Handle /rules command
function HandleChat(player)		
	local chatMsg = player:GetLastChat();
	
	if chatMsg == "/rules" then
		local rules = io.open("data/rules.txt", "r");
		if(rules == nil) then
			player:SendMessage("&eCould not read the rules file!");
			print("Could not read the rules file!");
		else
			for line in rules:lines() do
				player:SendMessage(line);
			end
			rules:close();
		end	
		player:SupressChat();
	end	
end
PLUGIN:AddHook("OnPlayerChat", "HandleChat");