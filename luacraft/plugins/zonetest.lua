PLUGIN:SetName("Zone Test");

PLUGIN:AddHook("OnBlockChange", "HandleBlocks");
function HandleBlocks(player)
	local LastBlock = player:GetLastBlock();
	player.LastBlock = player:GetLastBlock();
	if not player.UsingActionBlocks then return	end

	-- Remember actionblock
	player.ActionBlocks[3] = player.ActionBlocks[2];
	player.ActionBlocks[2] = player.ActionBlocks[1];
	player.ActionBlocks[1] = player:GetLastBlock();

	-- Dont place block
	player.ActionBlocks[1].blocktype = player.ActionBlocks[1].levelblock;
	SERVER:PlaceBlock(player.ActionBlocks[1], player:GetWorld());

	-- Send new block as lava
	player.ActionBlocks[1].blocktype = BLOCK_LAVA;
	player:SendBlockChange(player.ActionBlocks[1]);

	-- Clear old blocks
	if player.ActionBlocks[3] then
		player.ActionBlocks[3].blocktype = player.ActionBlocks[3].levelblock;
		player:SendBlockChange(player.ActionBlocks[3]);
	end

end

PLUGIN:AddChatCommand("a", "ToggleActionBlocks");
PLUGIN:AddChatCommand("action", "ToggleActionBlocks");
function ToggleActionBlocks(player, param)
	if player.UsingActionBlocks then
		-- Set state to false
		player.UsingActionBlocks = false;
		
		-- Clear old blocks
		if player.ActionBlocks[2] then
			player.ActionBlocks[2].blocktype = player.ActionBlocks[2].levelblock;
			player:SendBlockChange(player.ActionBlocks[2]);
		end
		if player.ActionBlocks[1] then
			player.ActionBlocks[1].blocktype = player.ActionBlocks[1].levelblock;
			player:SendBlockChange(player.ActionBlocks[1]);
		end
		player.ActionBlocks = nil;
		player:SendMessage("&eYou are no longer placing ActionBlocks");
	else
		-- Set state to true
		player.UsingActionBlocks = true;
		
		player.ActionBlocks = {};
		player:SendMessage("&eYou are now placing ActionBlocks");
	end
end

PLUGIN:AddChatCommand("draw", "Draw");
PLUGIN:AddChatCommand("d", "Draw");
function Draw(player, param)
	if not player.UsingActionBlocks or not player.ActionBlocks[1] or not player.ActionBlocks[2] then
		player:SendMessage("&eYou must place two ActionBlocks!");
		return;
	end
	
	player:SendMessage("&eDrawing...");
	
	local zone = SERVER:CreateZone(player:GetWorld(), player.ActionBlocks[1], player.ActionBlocks[2]);
	--if player.LastBlock.mode == 0 then
	--	player.LastBlock.blocktype = BLOCK_AIR;
	--end
		-- Commented that out - just use '/set air' instead before drawing
	if player.LastBlock.blocktype == BLOCK_STONE and player.CurBlock then
		if player.CurBlock == "water" then
			player.LastBlock.blocktype = BLOCK_WATER;
		elseif player.CurBlock == "air" then
			player.LastBlock.blocktype = BLOCK_AIR;
		elseif player.CurBlock == "lava" then
			player.LastBlock.blocktype = BLOCK_LAVA;
		elseif player.CurBlock == "grass" then
			player.LastBlock.blocktype = BLOCK_GRASS;
		elseif player.CurBlock == "opcrete" then
			player.LastBlock.blocktype = BLOCK_OPCRETE;
		end
	end
	if player.IsDense then
		player.LastBlock.blocktype = player.LastBlock.blocktype + 0x32;
	end
	zone:Fill(player.LastBlock.blocktype);
	SERVER:DeleteZone(zone);
	
	player:SendMessage("&eDrawing complete!");
	
	player.ActionBlocks[1].levelblock = player.LastBlock.blocktype;
	player.ActionBlocks[2].levelblock = player.LastBlock.blocktype;
	player.ActionBlocks[1].blocktype = BLOCK_LAVA;
	player.ActionBlocks[2].blocktype = BLOCK_LAVA;
	player:SendBlockChange(player.ActionBlocks[1]);
	player:SendBlockChange(player.ActionBlocks[2]);
end

PLUGIN:AddHook("OnPlayerJoin", "HandleJoin");
function HandleJoin(player)
	player.UsingActionBlocks = false;
end