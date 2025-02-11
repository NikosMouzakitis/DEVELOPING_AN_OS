#ifndef __STRLIB_H__
#define __STRLIB_H__

typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;



int strlen(char * src);
void memcpy(char *dest, char *src, int count);
void memset(void *dest, char val, u32 count);
char* itoa(int num, char* str, int base);
#endif
