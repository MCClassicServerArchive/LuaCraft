#ifndef _STRUCTS_H
#define _STRUCTS_H

struct position {
	unsigned short x, y, z;
	unsigned char heading, pitch;
};

struct block {
	unsigned short x, y, z;
	unsigned char blocktype, mode, levelblock;
};

struct NetworkReturn {
	int tmpSock;
	char *IP;
};

#endif