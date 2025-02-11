#include "strlib.h"

int strlen(char * src)
{
	unsigned int cnt;

	for( cnt = 0; src[cnt] != '\0'; cnt++)
		;

	return cnt;
}

//memset func.
void memset(void *dest, char val, u32 count){
    char *temp = (char*) dest;
    for (; count != 0; count --){
        *temp++ = val;
    }

}


void memcpy(char *dest, char *src, int count)
{
	int i;

	for(i = 0; i < count; i++)
		dest[i] = src[i];

}

char* itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers only if base is 10
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // Append negative sign for negative numbers
    if (isNegative) {
        str[i++] = '-';
    }

    // Null-terminate string
    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return str;
}

