PLUGIN:SetName("PrivateMessages");

-- Split a string into an array of words
function SplitWords(str)
  local t = {}
  local function helper(word) table.insert(t, word) return "" end
  if not str:gsub("%w+", helper):find"%S" then return t end
end

-- On player join
function HandleJoin(player)
	player:SendMessage("&aUse the &e/pm &acommand to send private messages!");
end
PLUGIN:AddHook("OnPlayerJoin", "HandleJoin");

-- On player chat
function HandleChat(player)		
	local chatMsg = player:GetLastChat();
	if chatMsg:sub(0,1) == "/" then
		if chatMsg:sub(2,4) == "pm " then
			if chatMsg == "/pm" then
				player:SendMessage("&eInvalid format! Use /pm <name> <message>");
			else
				chatMsg = chatMsg:sub(5);
				local name = SplitWords(chatMsg)[1];
				if chatMsg:sub(name:len() + 1):len() > 0 then
					local target = SERVER:GetPlayerByName(name);
					if target then
						local length = name:len() + 2;
						player:SendMessage("&eYour private message to "..target.Color..target:GetName().." &ehas been sent!");
						target:SendMessage("&4[PM] "..player:GetDisplayName()..": &7"..chatMsg:sub(name:len() + 2));
					else
						player:SendMessage("&ePlayer not found!");
					end
				else
					player:SendMessage("&eInvalid format! Use /pm <name> <message>");
				end
			end
		end
	end
	
end
PLUGIN:AddHook("OnPlayerChat", "HandleChat");

-- Add manual entry
function PluginsLoaded()
	if GetPluginByName("Manual") then
		local ok, msg = pcall(GetPluginByName("Manual").AddMan, "pm", "Use /pm <name> <message> to send that player a private message");
		if not ok then error(msg) end
	end
end
PLUGIN:AddHook("OnPluginsLoaded", "PluginsLoaded");