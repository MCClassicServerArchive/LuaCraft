-- On player chat
function HandleChat(player)		
	-- Get what they said
	local chatMsg = player:GetLastChat();
	
	-- Print their chat message to the console
	print(player:GetName()..": "..chatMsg);
	
	-- See if their message was a command
	if chatMsg:sub(0,1) == "/" then		
		-- Kick command
		if chatMsg:sub(2,5) == "kick" and player:GetOP() == false then
			player:SendMessage("&eYou must be an OP!");
		elseif chatMsg == "/kick" and player:GetOP() then
			player:SendMessage("&eInvalid format! Use /kick <name>");
		elseif chatMsg:sub(2,6) == "kick " then
			local target = SERVER:GetPlayerByName(chatMsg:sub(7));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				target:Kick("You were kicked by "..player:GetName());
			end
		end	
		
		-- Ban command
		if chatMsg:sub(2,4) == "ban" and player:GetOP() == false then
			player:SendMessage("&eYou must be an OP!");
		elseif chatMsg == "/ban" and player:GetOP() then
			player:SendMessage("&eInvalid format! Use /nab <name>");
		elseif chatMsg:sub(2,5) == "ban " then
			local target = SERVER:GetPlayerByName(chatMsg:sub(6));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				Ban(target);
			end
		end
		
		-- Teleport command
		if chatMsg:sub(2,3) == "tp" and player:GetOP() == false then
			player:SendMessage("&eYou must be an OP!");
		elseif chatMsg == "/tp" and player:GetOP() then
			player:SendMessage("&eInvalid format! Use /tp <name>");
		elseif chatMsg:sub(2,4) == "tp " then
			local target = SERVER:GetPlayerByName(chatMsg:sub(5));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				player:SendToPlayer(target);
				print("Player ["..player:GetName().."] teleported to player ["..target:GetName().."]");
			end
		end
		
		-- Bring command
		if chatMsg:sub(2,6) == "bring" and player:GetOP() == false then
			player:SendMessage("&eYou must be an OP!");
		elseif chatMsg == "/bring" and player:GetOP() then
			player:SendMessage("&eInvalid format! Use /bring <name>");
		elseif chatMsg:sub(2,7) == "bring " then
			local target = SERVER:GetPlayerByName(chatMsg:sub(8));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				target:SendToPlayer(player);
				print("Player ["..player:GetName().."] brought player ["..target:GetName().."] to them");
				target:SendMessage(player:GetDisplayName().." &ebrought you to them!");
			end
		end
		
		-- Spectate command
		if chatMsg:sub(2,5) == "spec" and player:GetOP() == false then
			player:SendMessage("&eYou must be an OP!");
		elseif chatMsg == "/spec" and player:GetOP() then
			if player.IsSpectating then
				player.IsSpectating = false;
				player:SendMessage("&eYou are no longer spectating "..SERVER:GetPlayerByName(player.SpectatingTarget):GetDisplayName());
				player:SetPos(player.PreSpecPos);
			else
				player:SendMessage("&eInvalid format! Use /spec <name>");
			end
		elseif chatMsg:sub(2,6) == "spec " then
			local target = SERVER:GetPlayerByName(chatMsg:sub(7));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				if player.IsSpectating then
					player.IsSpectating = false;
					player:SendMessage("&eYou are no longer spectating "..SERVER:GetPlayerByName(player.SpectatingTarget):GetDisplayname());
					player:SetPos(player.PreSpecPos);
				end
				player.IsSpectating = true;
				player.SpectatingTarget = target:GetName();
				player.PreSpecPos = player:GetPos();
				print("Player ["..player:GetName().."] is now spectating player ["..target:GetName().."]");
				player:SendMessage("&eYou are now spectating "..target:GetDisplayName());
			end
		end
		
		-- Op command
		if chatMsg == "/op" then
			player:SendMessage("&eInvalid format! Use /op <name>");
		elseif chatMsg:sub(2,4) == "op " then
			local target = SERVER:GetPlayerByName(chatMsg:sub(5));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				if target:GetOP() then
					player:SendMessage("&ePlayer is already an OP!");
				else
					target:SetOP(true);
					target:SetString("Color", "&9");
					target:SendMessage("&eYou are now an OP!");
					player:SendMessage("&eYou have just made "..target:GetName().." an OP!");
					print("Player ["..player:GetName().."] made player ["..target:GetName().."] an OP");
					
					-- Set their in-game name to be colored
					target:SetDisplayName(target:GetString("Color")..target:GetName());
				end
			end
		end
		
		-- Deop command
		if chatMsg == "/deop" then
			player:SendMessage("&eInvalid format! Use /deop <name>");
		elseif chatMsg:sub(2,6) == "deop " then
			local target = SERVER:GetPlayerByName(chatMsg:sub(7));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				if target:GetOP() then
					target:SetOP(false);
					target:SetString("Color", "&f");
					target:SendMessage("&eYou are no longer an OP!");
					player:SendMessage("&eYou have just removed "..target:GetName().." as an OP!");
					print("Player ["..player:GetName().."] removed player ["..target:GetName().."] as an OP");
					
					-- Set their in-game name to be colored
					target:SetDisplayName(target:GetString("Color")..target:GetName());
				else
					player:SendMessage("&ePlayer not an OP!");
				end
			end
		end
		
		-- Say command
		if chatMsg == "/say" then
			player:SendMessage("&eInvalid format! Use /say <message>");
		elseif chatMsg:sub(2,5) == "say " then
			SERVER:SendChat("&e"..chatMsg:sub(6):gsub("_","&"));
		end
		
		-- Me command
		if chatMsg == "/me" then
			player:SendMessage("&eInvalid format! Use /me <message>");
		elseif chatMsg:sub(2,4) == "me " then
			SERVER:SendChat("&d* "..player:GetName().." "..chatMsg:sub(5):gsub("_","&"));
		end
		
		-- Freeze command
		if chatMsg:sub(2,7) == "freeze" and player:GetOP() == false then
			player:SendMessage("&eYou must be an OP!");
		elseif chatMsg == "/freeze" and player:GetOP() then
			player:SendMessage("&eInvalid format! Use /freeze <name>");
		elseif chatMsg:sub(2,7) == "freeze" then
			local target = SERVER:GetPlayerByName(chatMsg:sub(9));
			if target == nil then
				player:SendMessage("&ePlayer not found!");
			else
				if target.IsFrozen then
					target.IsFrozen = false;
					target:SendMessage("&eYou were unfrozen by "..player:GetName());
					player:SendMessage("&eYou unfroze "..target:GetName());
					print("Player ["..player:GetName().."] unfroze player ["..target:GetName().."]");
				else
					target.IsFrozen = true;
					target:SendMessage("&eYou were frozen by "..player:GetName());
					player:SendMessage("&eYou froze "..target:GetName());
					print("Player ["..player:GetName().."] froze player ["..target:GetName().."]");
				end
			end
		end
		
		-- Dense command
		if chatMsg:sub(2, 6) == "dense" and player:GetOP() == false then
			player:SendMessage("&eYou must be an OP!");
		elseif chatMsg:sub(2, 6) == "dense" then
			if player.IsDense then
				player.IsDense = false;
				player:SendMessage("&eYou are no longer placing dense blocks.");
			else
				player.IsDense = true;
				player:SendMessage("&eYou are now placing dense blocks");
			end
		end
		
		-- Paint command
		if chatMsg:sub(2, 6) == "paint" then
			if player.IsPainting then
				player.IsPainting = false;
				player:SendMessage("&eYou are no longer painting.");
			else
				player.IsPainting = true;
				player:SendMessage("&eYou are now painting.");
			end
		end
		
		-- Save command
		if chatMsg:sub(2, 5) == "save" then
			if player:GetOP() then
				SERVER:GetLevelByName("main"):Save();
				SERVER:SendChat("&eLevel saved by "..player:GetDisplayName().." &eas &2server_level.dat", "main");
					print("Player ["..player:GetName().."] saved the map");
			else
				player:SendMessage("You must be an OP!");
			end
		end
		
		-- Set Spawn command
		if chatMsg:sub(2, 9) == "setspawn" then
			if player:GetOP() then
				SERVER:GetLevelByName("main"):SetSpawn(player:GetPos());
					print("Player ["..player:GetName().."] changed the spawn point");
			else
				player:SendMessage("You must be an OP!");
			end
		end
		
		-- You can add other core-gamemode commands here!
	else
		-- Send chat message to all other players
		if player:GetOP() then
			SERVER:SendChat(player:GetDisplayName().."&f: "..chatMsg:gsub("_","&"));
		else
			SERVER:SendChat(player:GetDisplayName().."&f: "..chatMsg);
		end
	end
	
end
GAMEMODE:AddHook("OnPlayerChat", "HandleChat");