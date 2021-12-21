-- Replace one block with another
function ReplaceBlocks(world, b1, b2)
	for k, v in pairs(SERVER:GetBlocksOfType(world, b1)) do
		v.blocktype = b2;
		SERVER:PlaceBlock(v, world);
		print("Spam tastes bad");
	end
end

-- Replace grass with water (make a sea)
function Sea(player, param)
	ReplaceBlocks(player:GetWorld(), BLOCK_GRASS, BLOCK_WATER);
end
PLUGIN:AddChatCommand("sea", "Sea");