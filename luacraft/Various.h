unsigned long gzip_compress(unsigned char *pDest, unsigned char *pSrc, unsigned long srcLen);
unsigned long gzip_decompress(unsigned char *pDest, unsigned char *pSrc, unsigned long srcLen);
unsigned char *memstr(unsigned char *mem, unsigned long memLen, unsigned char *subMem, unsigned long subMemLen);
int strlength(const char* str);
char *URIEncode(char *str);
typedef void* HANDLE;
char *stripColors(char *str);
void makeLower(char *str);