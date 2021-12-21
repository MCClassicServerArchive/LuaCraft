#ifndef _ZONE_H
#define _ZONE_H


/*	============================================
		The Zone class handles... Zones.
	============================================ */

#include "Level.h"
#include "Structs.h"

class Server;

class Zone {
public:
	Zone(Level *_level, block _block1, block _block2, Server *_server);

	Zone *next;

	void Fill(unsigned char blocktype);

private:
	Level *level;
	block block1;
	block block2;
	Server *server;
};

#endif