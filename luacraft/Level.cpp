#include <winsock2.h>
#include "Defines.h"
#include "Various.h"
#include "Console.h"
#include "Level.h"

Level::~Level() {
	if(serverLevel != NULL)
		free(serverLevel);
}

/* Will load up gzipped map fileName */
/* and store in unsigned char *serverLevel. */
//Level::Level(const char *fileName, bool restore) {
Level::Level(const char *tempMapName) {
	char *fileName = (char*)malloc(strlength(tempMapName)+5);
	sprintf(fileName, "maps/%s", tempMapName);

	this->FileName = fileName;

	unsigned char *compressedMap;
	FILE *map_in;

	//restoration = restore;
	restoration = false;

	if(restoration == false)
		Console::PrintText("Loading map \"%s\"", fileName);

	map_in = fopen(fileName, "rb");

	if(map_in == NULL) {
		if(restoration == false) {
			Console::PrintText("%s not found, generating a default flatmap.", fileName);
			newMap(128, 128, 128);
			saveMap(fileName);
			return;
		}

		else
			return;
	}

	fseek(map_in, -4, SEEK_END);
	fread(&decompressed_mapLen, 1, 4, map_in);
	compressed_mapLen = ftell(map_in);
	rewind(map_in);

	compressedMap = (unsigned char*) malloc(compressed_mapLen);
	fread(compressedMap, 1, compressed_mapLen, map_in);
	fclose(map_in);

	serverLevel = (unsigned char*) malloc(decompressed_mapLen);
	decompressed_mapLen = gzip_decompress(serverLevel, compressedMap, compressed_mapLen);
	free(compressedMap);
	fixMap();
	return;
}

/* Will "reformat" the old map, removing a large header */
/* and a smaller trailer. Will save us about 2 kB on large */
/* maps and even more when they are compressed, so saveMap */
/* wont have to work so hard. Also adds our mini-header */
/* containing 3 unsigned chars (identification), and another 13 unsigned chars */
/* for spawn/dimensions XYZ unsigned shorts */
bool Level::fixMap() {

	if(identifyMap() == false) {

		if(deserialize(serverLevel, decompressed_mapLen) == false) {
			if(restoration == false)
				Console::PrintText("Map is in a completely unknown format. We're sorry!");
			return false;
		}

		removeHeader();
		addHeader();
		if(restoration == false)
			Console::PrintText("Success! Corrected map WILL overwrite server_level.dat");
	}

	else {
		memcpy(&bDimX, serverLevel+3, 2);
		memcpy(&bDimY, serverLevel+5, 2);
		memcpy(&bDimZ, serverLevel+7, 2);
		memcpy(&spawnX, serverLevel+9, 2);
		memcpy(&spawnY, serverLevel+11, 2);
		memcpy(&spawnZ, serverLevel+13, 2);
		spawnRotX = serverLevel[15];
		spawnRotY = serverLevel[16];
		lDimX = ntohs(bDimX);
		lDimY = ntohs(bDimY);
		lDimZ = ntohs(bDimZ);
	}

	if(restoration == false)
		Console::PrintText("Successfully loaded map!");

	return true;
}

bool Level::identifyMap() {
	unsigned char ourHeader[] = { 0xBE, 0xEE, 0xEF };

	if(memcmp(serverLevel, ourHeader, sizeof(ourHeader)) == 0)
		return true;

	return false;
}

void Level::removeHeader() {
	unsigned char serializedEnd[] = { 0x00, 0x78, 0x70 };
	unsigned char *ptr, *tmp;

	ptr = memstr(serverLevel, decompressed_mapLen, serializedEnd, 3);
	tmp = ptr;

	while(ptr != NULL) {
		ptr += 3;
		tmp = ptr;
		ptr = memstr(ptr, decompressed_mapLen - (ptr - serverLevel), serializedEnd, sizeof(serializedEnd));
	}

	decompressed_mapLen -= (tmp - serverLevel);
	decompressed_mapLen += OUR_HEADERLEN;
	memmove(serverLevel, tmp - OUR_HEADERLEN, decompressed_mapLen);
	decompressed_mapLen = (OUR_HEADERLEN + 4) + (lDimX * lDimY * lDimZ);
	/* Avoid lame warning in g++ */
	serverLevel = (unsigned char*) realloc(serverLevel, decompressed_mapLen);
}

void Level::addHeader() {
	unsigned char ourHeader[] = { 0xBE, 0xEE, 0xEF };

	memcpy(serverLevel, ourHeader, sizeof(ourHeader));
	memcpy(serverLevel+3, &bDimX, sizeof(short));
	memcpy(serverLevel+5, &bDimY, sizeof(short));
	memcpy(serverLevel+7, &bDimZ, sizeof(short));
	memcpy(serverLevel+9, &spawnX, sizeof(short));
	memcpy(serverLevel+11, &spawnY, sizeof(short));
	memcpy(serverLevel+13, &spawnZ, sizeof(short));
	serverLevel[15] = spawnRotX;
	serverLevel[16] = spawnRotY;
}

bool Level::setSpawn(position pos) {

	/* If invalid position return false. */

	spawnX = pos.x;
	spawnY = pos.y;
	spawnZ = pos.z;
	spawnRotX = pos.heading;
	spawnRotY = pos.pitch;

	memcpy(serverLevel+9, &spawnX, sizeof(short));
	memcpy(serverLevel+11, &spawnY, sizeof(short));
	memcpy(serverLevel+13, &spawnZ, sizeof(short));
	serverLevel[15] = spawnRotX;
	serverLevel[16] = spawnRotY;

	return true;
}

bool Level::saveMap(const char *fileName) {
	unsigned char *compressedMap;
	FILE *map_out;

	compressedMap = (unsigned char*) malloc(compressed_mapLen + 16384);
	lvlLock();
	compressed_mapLen = gzip_compress(compressedMap, serverLevel, decompressed_mapLen);
	lvlUnlock();
	map_out = fopen(fileName, "wb");
	fwrite(compressedMap, 1, compressed_mapLen, map_out);
	fclose(map_out);
	free(compressedMap);
	return true;
}

bool Level::deserialize(unsigned char *Object, unsigned long objectLen) {
	unsigned long i, offset;
	unsigned long dx = 0, dy = 0, dz = 0;
	unsigned long sx = 0, sy = 0, sz = 0;
	unsigned short tmp;

	unsigned char serializedStart[] = { 0xAC, 0xED };
	unsigned char serializedEnd[] = { 0x78, 0x70 };
	unsigned char *temp;

	for(i = 0; i < objectLen; i++) {

		if(memcmp(Object+i, serializedStart, sizeof(serializedStart)) == 0) {

			if(restoration == false) {
				Console::PrintText("Map contains Java serialization.\n");
				Console::PrintText("Will attempt to deserialize, might result in crashes, and worse.\n");
			}

			/* Jump past Java-gaystuff and hope that it's constant in each map. */
			/* Or just jump by, and find out what purpose it fills later! */

			Object += i;
			Object += 6;
			memcpy(&tmp, Object, 2);
			Object += htons(tmp);
			Object += 13;

			/* We're certain that we're iterating over serialized memory. */
			/* That means there has to be a termination: serializedEnd. */
			/* Find it, to use as offset for each variable we extract. */

			for(temp = Object; ; temp++) {
				if(memcmp(temp, serializedEnd, sizeof(serializedEnd)) == 0) {
					temp += 2;
					break;
				}
			}

			offset = 0;

			for(;;) {
				if(*Object == 'Z')
					offset++;

				else if(*Object == 'I' || *Object == 'F')
					offset += 4;

				else if(*Object == 'J')
					offset += 8;

				Object++;
				memcpy(&tmp, Object, 2);
				Object += 2;

				if(memcmp(Object, "width", 5) == 0)
					memcpy(&dx, temp+offset-4, sizeof(long));

				else if(memcmp(Object, "depth", 5) == 0)
					memcpy(&dy, temp+offset-4, sizeof(long));

				else if(memcmp(Object, "height", 6) == 0)
					memcpy(&dz, temp+offset-4, sizeof(long));

				else if(memcmp(Object, "xSpawn", 6) == 0)
					memcpy(&sx, temp+offset-4, sizeof(long));

				else if(memcmp(Object, "ySpawn", 6) == 0)
					memcpy(&sy, temp+offset-4, sizeof(long));

				else if(memcmp(Object, "zSpawn", 6) == 0) {
					memcpy(&sz, temp+offset-4, sizeof(long));
					break;
				}

				Object += htons(tmp);
			}

			dx = ntohl(dx);
			dy = ntohl(dy);
			dz = ntohl(dz);
			sx = ntohl(sx);
			sy = ntohl(sy);
			sz = ntohl(sz);

			lDimX = (unsigned short) dx;
			lDimY = (unsigned short) dy;
			lDimZ = (unsigned short) dz;
			spawnX = (unsigned short) sx;
			spawnY = (unsigned short) sy;
			spawnZ = (unsigned short) sz;

			spawnX *= 32;
			spawnY *= 32;
			spawnZ *= 32;
			spawnX += 16;
			spawnZ += 16;

			bDimX = htons(lDimX);
			bDimY = htons(lDimY);
			bDimZ = htons(lDimZ);
			spawnX = htons(spawnX);
			spawnY = htons(spawnY);
			spawnZ = htons(spawnZ);

			return true;
		}
	}

	return false;
}

/* Creates a new flat map */
bool Level::newMap(unsigned short sizeX, unsigned short sizeY, unsigned short sizeZ) {
	unsigned long size;
	unsigned short i, j, k;

	while(sizeX % 8 != 0)
		sizeX++;

	while(sizeY % 8 != 0)
		sizeY++;

	while(sizeZ % 8 != 0)
		sizeZ++;

	size = sizeX * sizeY * sizeZ;

	decompressed_mapLen = (size + OUR_HEADERLEN + 4);
	compressed_mapLen = decompressed_mapLen;

	serverLevel = (unsigned char*) malloc(decompressed_mapLen);

	if(serverLevel == NULL) {
		Console::PrintText("Couldn't allocate enough memory to store the map!\n");
		return false;
	}

	memset(serverLevel, W_BLOCK_AIR, decompressed_mapLen);

	// Do this now, blockIndex() wants dimX, dimY and dimZ to be set
	lDimX = sizeX;
	lDimY = sizeY;
	lDimZ = sizeZ;
	spawnX = sizeX/2;
	spawnY = (sizeY+2)/2;
	spawnZ = sizeZ/2;
	spawnRotX = 0;
	spawnRotY = 0;

	spawnX *= 32;
	spawnY *= 32;
	spawnZ *= 32;
	spawnX += 16;
	spawnY += 16;

	bDimX = htons(lDimX);
	bDimY = htons(lDimY);
	bDimZ = htons(lDimZ);

	for(j = 0; j < (sizeY / 2) - 1; j++) {
		for(i = 0; i < sizeX; i++) {
			for(k = 0; k < sizeZ; k++) {
				setBlock(i, j, k, W_BLOCK_DIRT);
			}
		}
	}

	for(j = (sizeY / 2) - 1; j < (sizeY / 2); j++) {
		for(i = 0; i < sizeX; i++) {
			for(k = 0; k < sizeZ; k++) {
				setBlock(i, j, k, W_BLOCK_GRASS);
			}
		}
	}

	/* Add the size (dimX * Y * Z) to the header. */
	size = htonl(size);
	memcpy(serverLevel+OUR_HEADERLEN, &size, 4);

	/* This is our last endianshift. */
	spawnX = htons(spawnX);
	spawnY = htons(spawnY);
	spawnZ = htons(spawnZ);

	addHeader();
	return true;
}

void Level::Save() {
	this->saveMap(this->FileName);
}

void Level::SaveAs(const char *saveAsName) {
	char *fileName = (char*)malloc(strlength(saveAsName)+5);
	sprintf(fileName, "maps/%s", saveAsName);
	this->saveMap(fileName);
}

void Level::SetSpawn(position pos) {
	this->setSpawn(pos);
}