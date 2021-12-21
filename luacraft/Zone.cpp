#include "Various.h"
#include "Server.h"
#include "Zone.h"

Zone::Zone(Level *_level, block _block1, block _block2, Server *_server) {
	this->level = _level;
	this->block1 = _block1;
	this->block2 = _block2;
	this->server = _server;
	this->next = NULL;
}



void Zone::Fill(unsigned char blocktype) {
	block b;
	unsigned short temp;
	b.blocktype = blocktype;
	b.mode = 0;
	b.levelblock = 0;
	if(block1.x > block2.x) {
		temp = block1.x;
		block1.x = block2.x;
		block2.x = temp;
	}
	if(block1.y > block2.y) {
		temp = block1.y;
		block1.y = block2.y;
		block2.y = temp;
	}
	if(block1.z > block2.z) {
		temp = block1.z;
		block1.z = block2.z;
		block2.z = temp;
	}
	for(unsigned short x = this->block1.x; x <= this->block2.x; x++) {
		for(unsigned short y = this->block1.y; y <= this->block2.y; y++) {
			for(unsigned short z = this->block1.z; z <= this->block2.z; z++) {
				b.x = x;
				b.y = y;
				b.z = z;
				this->server->PlaceBlock(b, this->level->Name);
			}
		}
	}
}