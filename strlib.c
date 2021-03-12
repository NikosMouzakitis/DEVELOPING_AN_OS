#include "strlib.h"

int strlen(char * src)
{
	unsigned int cnt;

	for( cnt = 0; src[cnt] != '\0'; cnt++)
		;

	return cnt;
}

void memcpy(char *dest, char *src, int count)
{
	int i;

	for(i = 0; i < count; i++)
		dest[i] = src[i];

}
