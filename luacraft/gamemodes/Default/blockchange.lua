function HandleBlocks(player)
	block = player:GetLastBlock();
	
	if block.levelblock == BLOCK_INVALID then
		return; -- Invalid block position.
	end
	--Valid block, so continue.
	if block.mode == 0 then -- Block destroyed, type is the block destroyed.
		if block.levelblock > BLOCK_OBSIDIAN and player:GetOP() == false then -- Block is breakproof, and they are not an OP
			block.blocktype = block.levelblock;
			player:SendBlockChange(block);
			return;
		end 
		
		-- They are spectating or frozen (reset block)
		if player.IsSpectating or player.IsFrozen then
			block.blocktype = block.levelblock;
			player:SendBlockChange(block);
			return;
		end	
		
		-- Its a normal block, or they are an OP so they can delete it.
		if not player.IsPainting then
			block.blocktype = BLOCK_AIR;
		end	
		
		-- If it's a doublestair, just delete the "top step"
		if block.levelblock == BLOCK_DOUBLESTAIR or block.levelblock == (BLOCK_DOUBLESTAIR - 0x32) then
			block.blocktype = BLOCK_STAIR;
		end
		
		if player.IsDense then 
			block.blocktype = block.blocktype + 0x32;
		end
		
		if block.blocktype == BLOCK_STONE and player.CurBlock then -- Placing stone and they are set to place a special block
			if player.CurBlock == "water" then
				block.blocktype = BLOCK_WATER;
			elseif player.CurBlock == "air" then
				block.blocktype = BLOCK_AIR;
			elseif player.CurBlock == "lava" then
				block.blocktype = BLOCK_LAVA;
			elseif player.CurBlock == "grass" then
				block.blocktype = BLOCK_GRASS;
			elseif player.CurBlock == "opcrete" then
				block.blocktype = BLOCK_OPCRETE;
			end
		end
		
		SERVER:PlaceBlock(block, player:GetWorld());
	else -- Block placed, type is the block.
		if block.blocktype == BLOCK_STONE and player.CurBlock then -- Placing stone and they are set to place a special block
			if player.CurBlock == "water" then
				block.blocktype = BLOCK_WATER;
			elseif player.CurBlock == "air" then
				block.blocktype = BLOCK_AIR;
			elseif player.CurBlock == "lava" then
				block.blocktype = BLOCK_LAVA;
			elseif player.CurBlock == "grass" then
				block.blocktype = BLOCK_GRASS;
			elseif player.CurBlock == "opcrete" then
				block.blocktype = BLOCK_OPCRETE;
			end
		end
		
		if block.blocktype > BLOCK_ROCK and block.blocktype < BLOCK_SAND then -- Opcrete, water, or lava.
			if player:GetOP() == false and block.blocktype == BLOCK_OPCRETE then
				player:SendMessage("&eYou can not place this block!");
				if block.levelblock then
					block.blocktype = block.levelblock;
				else
					block.blocktype = BLOCK_AIR;
				end
				player:SendBlockChange(block);
			end
		end
		
		if block.levelblock > BLOCK_OBSIDIAN and player:GetOP() == false then -- Block is dense and they are not an OP
			block.blocktype = block.levelblock;
			player:SendBlockChange(block);
			return;
		end
		
		if player.IsDense then
			block.blocktype = block.blocktype + 0x32;
		end
		
		if block.blocktype == BLOCK_STAIR then -- Simple stair physics
			local below = SERVER:GetBlock(player:GetWorld(), block.x, block.y-1, block.z);
			if below.blocktype == BLOCK_STAIR then
				below.blocktype = BLOCK_DOUBLESTAIR;
				SERVER:PlaceBlock(below, player:GetWorld());
				block.blocktype = BLOCK_AIR;
			end
		end
		
		SERVER:PlaceBlock(block, player:GetWorld());
	end
end
GAMEMODE:AddHook("OnBlockChange", "HandleBlocks");