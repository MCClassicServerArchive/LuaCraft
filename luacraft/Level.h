#ifndef _LEVEL_H
#define _LEVEL_H


/*	============================================
		The Level class contains represents
		a map. It contains the data, IO
		operations, and other functions relating
		to a map.
	============================================ */

/* BLOCK TYPES */
#define W_BLOCK_AIR		  0x00
#define W_BLOCK_STONE	  0x01
#define W_BLOCK_GRASS	  0x02
#define W_BLOCK_DIRT	  0x03
#define W_BLOCK_COBBLE	  0x04
#define W_BLOCK_WOOD	  0x05
#define W_BLOCK_PLANT	  0x06
#define W_BLOCK_ROCK	  0x07
#define W_BLOCK_WATER	  0x08
#define W_BLOCK_SWATER	  0x09
#define W_BLOCK_LAVA	  0x0A
#define W_BLOCK_SLAVA	  0x0B
#define W_BLOCK_SAND	  0x0C
#define W_BLOCK_GRAVEL	  0x0D
#define W_BLOCK_GOLDORE	  0x0E
#define W_BLOCK_IRONORE	  0x0F
#define W_BLOCK_COAL	  0x10
#define W_BLOCK_TREE	  0x11
#define W_BLOCK_LEAVES	  0x12
#define W_BLOCK_SPONGE	  0x13
#define W_BLOCK_GLASS	  0x14
#define W_BLOCK_REDC	  0x15
#define W_BLOCK_ORANGEC	  0x16
#define W_BLOCK_YELLOWC	  0x17
#define W_BLOCK_LGREENC	  0x18
#define W_BLOCK_GREENC	  0x19
#define W_BLOCK_AGREENC	  0x1A
#define W_BLOCK_CYANC	  0x1B
#define W_BLOCK_BLUEC	  0x1C
#define W_BLOCK_PURPLEC	  0x1D
#define W_BLOCK_INDIGOC	  0x1E
#define W_BLOCK_VIOLETC   0x1F
#define W_BLOCK_MAGENTAC  0x20
#define W_BLOCK_PINKC	  0x21
#define W_BLOCK_BLACKC	  0x22
#define W_BLOCK_GRAYC	  0x23
#define W_BLOCK_WHITEC	  0x24
#define W_BLOCK_YFLOWER	  0x25
#define W_BLOCK_RFLOWER	  0x26
#define W_BLOCK_BSHROOM	  0x27
#define W_BLOCK_RSHROOM	  0x28
#define W_BLOCK_GOLD	  0x29
#define W_BLOCK_IRON	  0x2A
#define W_BLOCK_DOUBSTAIR 0x2B
#define W_BLOCK_STAIR	  0x2C
#define W_BLOCK_BRICK	  0x2D
#define W_BLOCK_TNT		  0x2E
#define W_BLOCK_BOOKS	  0x2F
#define W_BLOCK_MOSSCOB	  0x30
#define W_BLOCK_OBSIDIAN  0x31

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Structs.h"
#include "Defines.h"

class Level {

public:

	//Level(const char *fileName, bool restore);
	Level(const char *tempMapName);
	~Level();

	//bool loadMap(const char *fileName, bool restore);
	bool saveMap(const char *fileName);
	bool setSpawn(position pos);

	/* Inlines for speed. */

	inline bool validBlock(unsigned short x, unsigned short y, unsigned short z) {
		if(x > lDimX)
			return false;
		if(y >= lDimY)
			return false;
		if(z > lDimZ)
			return false;

		return true;
	}

	inline unsigned long blockIndex(unsigned short x, unsigned short y, unsigned short z) {
		return ((y * lDimZ + z) * lDimX + x) + OUR_HEADERLEN + 4;
	}

	inline void setBlock(unsigned long offset, unsigned char type) {
		serverLevel[offset] = type;
	}

	inline void setBlock(unsigned short x, unsigned short y, unsigned short z, unsigned char type) {
		serverLevel[blockIndex(x, y, z)] = type;
	}

	inline unsigned char getBlock(unsigned short x, unsigned short y, unsigned short z) {
		return serverLevel[blockIndex(x, y, z)];
	}

	inline unsigned long getCompressedMapLen() {
		return compressed_mapLen;
	}

	inline void setCompressedMapLen(unsigned long newLen) {
		compressed_mapLen = newLen;
	}

	inline unsigned long getDecompressedMapLen() {
		return decompressed_mapLen;
	}

	inline unsigned short getDimX() {
		return lDimX;
	}

	inline unsigned short getDimY() {
		return lDimY;
	}
	
	inline unsigned short getDimZ() {
		return lDimZ;
	}
	
	inline unsigned short getSpawnX() {
		return spawnX;
	}
	
	inline unsigned short getSpawnY() {
		return spawnY;
	}
	
	inline unsigned short getSpawnZ() {
		return spawnZ;
	}

	inline unsigned char getRotX() {
		return spawnRotX;
	}

	inline unsigned char getRotY() {
		return spawnRotY;
	}

	inline unsigned char *getServerLevel() {
		return serverLevel;
	}

	inline unsigned char *getServerLevel_safe() {
		unsigned long i;
		unsigned char *map;
		
		serverLevel_safe = (unsigned char*) malloc(decompressed_mapLen-(OUR_HEADERLEN));
		map = serverLevel + OUR_HEADERLEN;

		memcpy(serverLevel_safe, map, sizeof(long));

		for(i = 4; i < decompressed_mapLen - OUR_HEADERLEN; i++) {
			serverLevel_safe[i] = map[i];
			if(serverLevel_safe[i] > 0x31)
				serverLevel_safe[i] -= 0x32;
		}

		return serverLevel_safe;
	}

	inline void freeServerLevel_safe() {
		free(serverLevel_safe);
	}

	inline void lvlLock() {
		//lock(mutexServerLevel);
	}

	inline void lvlUnlock() {
		//unlock(mutexServerLevel);
	}

	char *Name;
	char *FileName;

	void Save();
	void SaveAs(const char *saveAsName);
	void SetSpawn(position pos);

private:
	
	bool fixMap();
	bool identifyMap();
	bool deserialize(unsigned char *Object, unsigned long objectLen);
	void removeHeader();
	void addHeader();

	bool newMap(unsigned short sizeX, unsigned short sizeY, unsigned short sizeZ);

	unsigned long compressed_mapLen;
	unsigned long decompressed_mapLen;

	/* Little and big endian. */
	/* Better then doing endianshifts all the f'n time... */		// <- I lol'd.
	unsigned short bDimX, bDimY, bDimZ;
	unsigned short lDimX, lDimY, lDimZ;
	unsigned short spawnX, spawnY, spawnZ;
	unsigned char spawnRotX, spawnRotY;

	unsigned char *serverLevel;
	unsigned char *serverLevel_safe;

	bool restoration;

	//mutex_t mutexServerLevel;

};

#endif