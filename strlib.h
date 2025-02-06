#ifndef __STRLIB_H__
#define __STRLIB_H__

int strlen(char * src);
void memcpy(char *dest, char *src, int count);

char* itoa(int num, char* str, int base);
#endif
