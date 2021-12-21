#pragma comment(lib, "libs/zdll.lib")

#define CHUNK 262144

#include "libs/zlib.h"
#include <cstring>
#include <cctype>
#include "Various.h"

// Return the length of a string
int strlength(const char* str) {
	int i;
	for(i=0; str[i]; i++);
	return i;
}

/* Compress pSrc and save in pDest. */
/* Will not allocate memory for you. */
unsigned long gzip_compress(unsigned char *pDest, unsigned char *pSrc, unsigned long srcLen) {
	unsigned long have;
	unsigned char out[CHUNK];
	z_stream stream;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY);

	stream.avail_in = srcLen;
	stream.next_in = pSrc;

	do {
		stream.avail_out = sizeof(out);
		stream.next_out = out;
		deflate(&stream, Z_FINISH);
		have = sizeof(out) - stream.avail_out;
		memcpy(pDest, out, have);
		pDest = pDest + have;
	} while(stream.avail_out == 0);

	deflateEnd(&stream);
	return stream.total_out;
}

/* Decompress pSrc and save in pDest */
/* Will not allocate memory for you. */
unsigned long gzip_decompress(unsigned char *pDest, unsigned char *pSrc, unsigned long srcLen) {
	unsigned long have;
	unsigned char out[CHUNK];
	z_stream stream;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	inflateInit2(&stream, 15+32);

	stream.avail_in = srcLen;
	stream.next_in = pSrc;

	do {
		stream.avail_out = sizeof(out);
		stream.next_out = out;
		inflate(&stream, Z_NO_FLUSH);
		have = sizeof(out) - stream.avail_out;
		memcpy(pDest, out, have);
		pDest = pDest + have;
	} while(stream.avail_out == 0);

	inflateEnd(&stream);
	return stream.total_out;
}

/* Will search for any passed memory subMem in memory mem */
/* Kinda strstr but without the null-termination. */
/* If subMem isn't found it returns NULL */
unsigned char *memstr(unsigned char *mem, unsigned long memLen, unsigned char *subMem, unsigned long subMemLen) {
	unsigned long tmpLen;
	unsigned char *match;

	match = (unsigned char *) memchr(mem, *subMem, memLen);

	while(match != NULL) {
		if(memcmp(match, subMem, subMemLen) == 0)
			return match;

		/* Subtract the number of unsigned chars we've been searching through. */
		match++;
		tmpLen = (memLen - (match - mem));
		match = (unsigned char*) memchr(match, *subMem, tmpLen);
	}

	return NULL;
}