#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *itoa_16(int n);

const unsigned int k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

const unsigned int r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
                     };

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void to_bytes(unsigned int val, unsigned char *bytes)
{
    bytes[0] = (unsigned char) val;
    bytes[1] = (unsigned char) (val >> 8);
    bytes[2] = (unsigned char) (val >> 16);
    bytes[3] = (unsigned char) (val >> 24);
}

unsigned int to_int32(const unsigned char *bytes)
{
    return (unsigned int) bytes[0]
           | ((unsigned int) bytes[1] << 8)
           | ((unsigned int) bytes[2] << 16)
           | ((unsigned int) bytes[3] << 24);
}

void md5(const unsigned char *initial_msg, size_t initial_len, unsigned char *digest)
{
    unsigned int h0, h1, h2, h3;

    unsigned char *msg = NULL;

    size_t new_len, offset;
    unsigned int w[16];
    unsigned int a, b, c, d, i, f, g, temp;

    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;

    for (new_len = initial_len + 1; new_len % (512/8) != 448/8; new_len++)
        ;

    msg = malloc(new_len + 8);
    memcpy(msg, initial_msg, initial_len);//////////////////////////////////
    msg[initial_len] = 0x80;
    for (offset = initial_len + 1; offset < new_len; offset++)
        msg[offset] = 0;

    to_bytes(initial_len*8, msg + new_len);
    to_bytes(initial_len>>29, msg + new_len + 4);

    for(offset=0; offset<new_len; offset += (512/8)) {

        for (i = 0; i < 16; i++)
            w[i] = to_int32(msg + offset + i*4);

        a = h0;
        b = h1;
        c = h2;
        d = h3;

        for(i = 0; i<64; i++) {

            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;

        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;

    }

    free(msg);

    to_bytes(h0, digest);
    to_bytes(h1, digest + 4);
    to_bytes(h2, digest + 8);
    to_bytes(h3, digest + 12);
}

char *get_crypto_key(char *msg)
{
    char *tmp;
    size_t len;
    int i;
    unsigned char result[16];
    char *ret;

    ret = (char *)malloc(64);
    len = strlen(msg);///////////////////////////////////

    for (i = 0; i < 1000; i++) {
        md5((unsigned char *)msg, len, result);
    }

    for (i = 0; i < 16; i++)
    {
        tmp = itoa_16((unsigned char)result[i]);
        if (tmp[0] == 0)
            tmp[0] = 48;
        if (tmp[1] == 0)
            tmp[1] = 48;
        ret[i * 2] = tmp[0];
        ret[i * 2 + 1] = tmp[1];
        free (tmp);

    }
    ret[32] = 0;


    return (ret);
}
