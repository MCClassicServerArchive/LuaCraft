/*
 * Functions to compute MD5 message digest of files or memory blocks
 * according to the definition of MD5 in RFC 1321 from April 1992.
 * Copyright (C) 1995, 1996 Free Software Foundation, Inc.  NOTE: The
 * canonical source of this file is maintained with the GNU C Library.
 * Bugs can be reported to bug-glibc@prep.ai.mit.edu.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.
 * Modified by Gray Watson <http://256.com/gray/>, 1997.
 *
 * $Id: md5.c,v 1.7 2006/03/05 15:38:09 gray Exp $
 */

#include <cstring>
#include <climits>
#include "md5.h"

static void process_block(md5_t *md5_p, const void *buffer, const unsigned int buf_len) {
	unsigned int correct[16];
	const void *buf_p = buffer, *end_p;
	unsigned int words_n;
	unsigned int A, B, C, D;
	words_n = buf_len / sizeof(unsigned int);
	end_p = (char *)buf_p + words_n * sizeof(unsigned int);

	A = md5_p->md_A;
	B = md5_p->md_B;
	C = md5_p->md_C;
	D = md5_p->md_D;

	if(md5_p->md_total[0] > UINT_MAX - buf_len) {
		md5_p->md_total[1]++;
		md5_p->md_total[0] -= (UINT_MAX + 1 - buf_len);
	}
	else {
		md5_p->md_total[0] += buf_len;
	}

	while(buf_p < end_p) {
		unsigned int	A_save, B_save, C_save, D_save;
		unsigned int	*corr_p = correct;

		A_save = A;
		B_save = B;
		C_save = C;
		D_save = D;

		OP1(A, B, C, D, buf_p, corr_p,	7, 0xd76aa478);
		OP1(D, A, B, C, buf_p, corr_p, 12, 0xe8c7b756);
		OP1(C, D, A, B, buf_p, corr_p, 17, 0x242070db);
		OP1(B, C, D, A, buf_p, corr_p, 22, 0xc1bdceee);
		OP1(A, B, C, D, buf_p, corr_p,	7, 0xf57c0faf);
		OP1(D, A, B, C, buf_p, corr_p, 12, 0x4787c62a);
		OP1(C, D, A, B, buf_p, corr_p, 17, 0xa8304613);
		OP1(B, C, D, A, buf_p, corr_p, 22, 0xfd469501);
		OP1(A, B, C, D, buf_p, corr_p,	7, 0x698098d8);
		OP1(D, A, B, C, buf_p, corr_p, 12, 0x8b44f7af);
		OP1(C, D, A, B, buf_p, corr_p, 17, 0xffff5bb1);
		OP1(B, C, D, A, buf_p, corr_p, 22, 0x895cd7be);
		OP1(A, B, C, D, buf_p, corr_p,	7, 0x6b901122);
		OP1(D, A, B, C, buf_p, corr_p, 12, 0xfd987193);
		OP1(C, D, A, B, buf_p, corr_p, 17, 0xa679438e);
		OP1(B, C, D, A, buf_p, corr_p, 22, 0x49b40821);

		OP234(FG, A, B, C, D, correct[	1],	5, 0xf61e2562);
		OP234(FG, D, A, B, C, correct[	6],	9, 0xc040b340);
		OP234(FG, C, D, A, B, correct[ 11], 14, 0x265e5a51);
		OP234(FG, B, C, D, A, correct[	0], 20, 0xe9b6c7aa);
		OP234(FG, A, B, C, D, correct[	5],	5, 0xd62f105d);
		OP234(FG, D, A, B, C, correct[ 10],	9, 0x02441453);
		OP234(FG, C, D, A, B, correct[ 15], 14, 0xd8a1e681);
		OP234(FG, B, C, D, A, correct[	4], 20, 0xe7d3fbc8);
		OP234(FG, A, B, C, D, correct[	9],	5, 0x21e1cde6);
		OP234(FG, D, A, B, C, correct[ 14],	9, 0xc33707d6);
		OP234(FG, C, D, A, B, correct[	3], 14, 0xf4d50d87);
		OP234(FG, B, C, D, A, correct[	8], 20, 0x455a14ed);
		OP234(FG, A, B, C, D, correct[ 13],	5, 0xa9e3e905);
		OP234(FG, D, A, B, C, correct[	2],	9, 0xfcefa3f8);
		OP234(FG, C, D, A, B, correct[	7], 14, 0x676f02d9);
		OP234(FG, B, C, D, A, correct[ 12], 20, 0x8d2a4c8a);

		OP234(FH, A, B, C, D, correct[	5],	4, 0xfffa3942);
		OP234(FH, D, A, B, C, correct[	8], 11, 0x8771f681);
		OP234(FH, C, D, A, B, correct[ 11], 16, 0x6d9d6122);
		OP234(FH, B, C, D, A, correct[ 14], 23, 0xfde5380c);
		OP234(FH, A, B, C, D, correct[	1],	4, 0xa4beea44);
		OP234(FH, D, A, B, C, correct[	4], 11, 0x4bdecfa9);
		OP234(FH, C, D, A, B, correct[	7], 16, 0xf6bb4b60);
		OP234(FH, B, C, D, A, correct[ 10], 23, 0xbebfbc70);
		OP234(FH, A, B, C, D, correct[ 13],	4, 0x289b7ec6);
		OP234(FH, D, A, B, C, correct[	0], 11, 0xeaa127fa);
		OP234(FH, C, D, A, B, correct[	3], 16, 0xd4ef3085);
		OP234(FH, B, C, D, A, correct[	6], 23, 0x04881d05);
		OP234(FH, A, B, C, D, correct[	9],	4, 0xd9d4d039);
		OP234(FH, D, A, B, C, correct[ 12], 11, 0xe6db99e5);
		OP234(FH, C, D, A, B, correct[ 15], 16, 0x1fa27cf8);
		OP234(FH, B, C, D, A, correct[	2], 23, 0xc4ac5665);

		OP234(FI, A, B, C, D, correct[	0],	6, 0xf4292244);
		OP234(FI, D, A, B, C, correct[	7], 10, 0x432aff97);
		OP234(FI, C, D, A, B, correct[ 14], 15, 0xab9423a7);
		OP234(FI, B, C, D, A, correct[	5], 21, 0xfc93a039);
		OP234(FI, A, B, C, D, correct[ 12],	6, 0x655b59c3);
		OP234(FI, D, A, B, C, correct[	3], 10, 0x8f0ccc92);
		OP234(FI, C, D, A, B, correct[ 10], 15, 0xffeff47d);
		OP234(FI, B, C, D, A, correct[	1], 21, 0x85845dd1);
		OP234(FI, A, B, C, D, correct[	8],	6, 0x6fa87e4f);
		OP234(FI, D, A, B, C, correct[ 15], 10, 0xfe2ce6e0);
		OP234(FI, C, D, A, B, correct[	6], 15, 0xa3014314);
		OP234(FI, B, C, D, A, correct[ 13], 21, 0x4e0811a1);
		OP234(FI, A, B, C, D, correct[	4],	6, 0xf7537e82);
		OP234(FI, D, A, B, C, correct[ 11], 10, 0xbd3af235);
		OP234(FI, C, D, A, B, correct[	2], 15, 0x2ad7d2bb);
		OP234(FI, B, C, D, A, correct[	9], 21, 0xeb86d391);

		A += A_save;
		B += B_save;
		C += C_save;
		D += D_save;
	}

	md5_p->md_A = A;
	md5_p->md_B = B;
	md5_p->md_C = C;
	md5_p->md_D = D;
}

static void md5_get_result(const md5_t *md5_p, void *result) {
	unsigned int	hold;
	void		*res_p = result;

	hold = SWAP(md5_p->md_A);
	memcpy(res_p, &hold, sizeof(unsigned int));
	res_p = (char *)res_p + sizeof(unsigned int);

	hold = SWAP(md5_p->md_B);
	memcpy(res_p, &hold, sizeof(unsigned int));
	res_p = (char *)res_p + sizeof(unsigned int);

	hold = SWAP(md5_p->md_C);
	memcpy(res_p, &hold, sizeof(unsigned int));
	res_p = (char *)res_p + sizeof(unsigned int);

	hold = SWAP(md5_p->md_D);
	memcpy(res_p, &hold, sizeof(unsigned int));
}

static void md5_init(md5_t *md5_p) {
	md5_p->md_A = 0x67452301;
	md5_p->md_B = 0xefcdab89;
	md5_p->md_C = 0x98badcfe;
	md5_p->md_D = 0x10325476;

	md5_p->md_total[0] = 0;
	md5_p->md_total[1] = 0;
	md5_p->md_buf_len = 0;
}

static void md5_process(md5_t *md5_p, const void *buffer, const unsigned int buf_len) {
	unsigned int	len = buf_len;
	unsigned int	in_block, add;

	if (md5_p->md_buf_len > 0) {
		
		in_block = md5_p->md_buf_len;
		if (in_block + len > sizeof(md5_p->md_buffer)) {
			add = sizeof(md5_p->md_buffer) - in_block;
		}
		else {
			add = len;
		}

		memcpy (md5_p->md_buffer + in_block, buffer, add);
		md5_p->md_buf_len += add;
		in_block += add;

		if (in_block > MD5_BLOCK_SIZE) {
			process_block (md5_p, md5_p->md_buffer, in_block & ~BLOCK_SIZE_MASK);
			memcpy (md5_p->md_buffer,
				md5_p->md_buffer + (in_block & ~BLOCK_SIZE_MASK),
				in_block & BLOCK_SIZE_MASK);
			md5_p->md_buf_len = in_block & BLOCK_SIZE_MASK;
		}

		buffer = (const char *)buffer + add;
		len -= add;
	}

	if (len > MD5_BLOCK_SIZE) {
		process_block (md5_p, buffer, len & ~BLOCK_SIZE_MASK);
		buffer = (const char *) buffer + (len & ~BLOCK_SIZE_MASK);
		len &= BLOCK_SIZE_MASK;
	}

	if (len > 0) {
		memcpy (md5_p->md_buffer, buffer, len);
		md5_p->md_buf_len = len;
	}
}

static void md5_finish(md5_t *md5_p, void *signature) {
	unsigned int	bytes, hold;
	int		pad;

	bytes = md5_p->md_buf_len;

	if (md5_p->md_total[0] > UINT_MAX - bytes) {
		md5_p->md_total[1]++;
		md5_p->md_total[0] -= (UINT_MAX + 1 - bytes);
	}
	else {
		md5_p->md_total[0] += bytes;
	}

	pad = MD5_BLOCK_SIZE - (sizeof(unsigned int) * 2) - bytes;
	if (pad <= 0) {
		pad += MD5_BLOCK_SIZE;
	}

	if (pad > 0) {
		md5_p->md_buffer[bytes] = (unsigned char)0x80;
		if (pad > 1) {
			memset (md5_p->md_buffer + bytes + 1, 0, pad - 1);
		}
		bytes += pad;
	}

	hold = SWAP((md5_p->md_total[0] & 0x1FFFFFFF) << 3);
	memcpy(md5_p->md_buffer + bytes, &hold, sizeof(unsigned int));
	bytes += sizeof(unsigned int);

	hold = SWAP((md5_p->md_total[1] << 3) |
				((md5_p->md_total[0] & 0xE0000000) >> 29));
	memcpy(md5_p->md_buffer + bytes, &hold, sizeof(unsigned int));
	bytes += sizeof(unsigned int);

	process_block(md5_p, md5_p->md_buffer, bytes);
	md5_get_result(md5_p, signature);
}

void md5_buffer(const char *buffer, const unsigned int buf_len, void *signature) {
	md5_t md5;

	md5_init(&md5);
	md5_process(&md5, buffer, buf_len);
	md5_finish(&md5, signature);
}

void md5_sig_to_string(void *signature, char *str, const int str_len) {
	unsigned char *sig_p;
	char *str_p, *max_p;
	unsigned int high, low;
	
	str_p = str;
	max_p = str + str_len;
	
	for (sig_p = (unsigned char *) signature; sig_p < (unsigned char *) signature + MD5_SIZE; sig_p++) {
		high = *sig_p / 16;
		low = *sig_p % 16;

		if (str_p + 1 >= max_p) {
			break;
		}

		*str_p++ = HEX_STRING[high];
		*str_p++ = HEX_STRING[low];
	}

	if (str_p < max_p) {
		*str_p++ = '\0';
	}
}