#ifndef _MD5_H
#define _MD5_H

#define MD5_SIZE 16
#define MAX_md5uint ((unsigned int) 4294967295U)
#define MD5_BLOCK_SIZE 64
#define HEX_STRING	"0123456789abcdef"
#define BLOCK_SIZE_MASK	(MD5_BLOCK_SIZE - 1)
#define SWAP(n)	(n)
#define FF(b, c, d)	(d ^ (b & (c ^ d)))
#define FG(b, c, d)	FF(d, b, c)
#define FH(b, c, d)	(b ^ c ^ d)
#define FI(b, c, d)	(c ^ (b | ~d))
#define CYCLIC(w, s) ((w << s) | (w >> (32 - s)))

#define OP1(a, b, c, d, b_p, c_p, s, T) \
	do { \
		memcpy(c_p, b_p, sizeof(unsigned int)); \
		*c_p = SWAP(*c_p); \
		a += FF (b, c, d) + *c_p + T; \
		a = CYCLIC (a, s); \
		a += b; \
		b_p = (char *)b_p + sizeof(unsigned int); \
		c_p++; \
	} while(0)

#define OP234(FUNC, a, b, c, d, k, s, T) \
	do { \
		a += FUNC (b, c, d) + k + T; \
		a = CYCLIC (a, s); \
		a += b; \
	} while(0)

typedef struct {
	unsigned int md_A;
	unsigned int md_B;
	unsigned int md_C;
	unsigned int md_D;

	unsigned int md_total[2];
	unsigned int md_buf_len;
	char md_buffer[MD5_BLOCK_SIZE * 2];
} md5_t;

static void md5_init(md5_t *md5_p);
static void md5_process(md5_t *md5_p, const void *buffer, const unsigned int buf_len);
static void md5_finish(md5_t *md5_p, void *signature);
void md5_buffer(const char *buffer, const unsigned int buf_len, void *signature);
void md5_sig_to_string(void *signature, char *str, const int str_len);

#endif