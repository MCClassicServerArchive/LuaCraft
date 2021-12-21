function HandleBlocks(player)
	block = player:GetLastBlock();
	
	if block.levelblock == 128 then
		return; -- Invalid block position.
	end
	--Valid block, so continue.
	if block.mode == 0x00 then -- Block destroyed, type is the block destroyed.
		if block.levelblock > 0x31 and player:GetOP() == false then -- Block is breakproof, and they are not an OP
			block.blocktype = block.levelblock;
			player:SendBlockChange(block);
			return;
		end 
		
		-- They are spectating (reset block)
		if player:GetBool("IsSpectating") then
			block.blocktype = block.levelblock;
			player:SendBlockChange(block);
			return;
		end
		
		-- Its a normal block, or they are an OP so they can delete it.
		if not player.IsPainting then
			block.blocktype = 0x00;
		end
		
		if player.IsDense then 
			block.blocktype = block.blocktype + 0x32;
		end
		
		SERVER:sendBlock(block, player:GetWorld());
	else -- Block placed, type is the block.
		if block.blocktype > 0x06 and block.blocktype < 0x0C then -- Opcrete, water, or lava.
			if not player:GetOP() then
				player:SendMessage("&eYou can not place this block!");
				block.blocktype = block.leveltype;
				player:SendBlockChange(block);
			end
		end
		
		if block.levelblock > 0x31 and player:GetOP() == false then -- Block is dense and they are not an OP
			block.blocktype = block.levelblock;
			player:SendBlockChange(block);
			return;
		end
		
		if player:GetBool("densing") then
			block.blocktype = block.blocktype + 0x32;
		end
		
		SERVER:sendBlock(block, player:GetWorld());
	end
end
GAMEMODE:AddHook("OnBlockChange", "HandleBlocks");