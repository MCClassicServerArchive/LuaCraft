PLUGIN:SetName("PrivateMessages");

-- Split a string into substrings by pat
function split(str, pat)
	local t = {};
	local fpat = "(.-)"..pat;
	local last_end = 1;
	local s, e, cap = str:find(fpat, 1);
	while s do
		if s ~= 1 or cap ~= "" then
			table.insert(t,cap);
		end
		last_end = e+1;
		s, e, cap = str:find(fpat, last_end);
	end
	if last_end <= #str then
		cap = str:sub(last_end);
		table.insert(t, cap);
	end
	return t;
end

-- On player join
function HandleJoin(player)
	player:SendMessage("&aUse the &e/pm &acommand to send private messages!");
end
PLUGIN:AddHook("OnPlayerJoin", "HandleJoin");

-- On chat command
function Message(player, param)		
	if param == "" then
		player:SendMessage("&eInvalid format! Use /pm <name> <message>");
		return;
	end
	local name = split(param, " ")[1];
	if param:sub(name:len() + 1):len() > 0 then
		local target = SERVER:GetPlayerByName(name);
		if target then
			local length = name:len() + 2;
			player:SendMessage("&eYour private message to "..target.Color..target:GetName().." &ehas been sent!");
			target:SendMessage("&4[PM] "..player:GetDisplayName()..": &7"..param:sub(name:len() + 2));
		else
			player:SendMessage("&ePlayer not found!");
		end
	else
		player:SendMessage("&eInvalid format! Use /pm <name> <message>");
	end
end
PLUGIN:AddChatCommand("pm", "Message");

-- Add manual entry
function PluginsLoaded()
	if SERVER:GetPluginByName("Manual") then
		local ok, msg = pcall(GetPluginByName("Manual").AddMan, "pm", "Use /pm <name> <message> to send that player a private message");
		if not ok then error(msg) end
	end
end
PLUGIN:AddHook("OnPluginsLoaded", "PluginsLoaded");