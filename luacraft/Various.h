unsigned long gzip_compress(unsigned char *pDest, unsigned char *pSrc, unsigned long srcLen);
unsigned long gzip_decompress(unsigned char *pDest, unsigned char *pSrc, unsigned long srcLen);
unsigned char *memstr(unsigned char *mem, unsigned long memLen, unsigned char *subMem, unsigned long subMemLen);
int strlength(const char* str);
typedef void* HANDLE;