#include <stddef.h>

// get the length of a string
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

// copy a string from src to dest
char* strcpy(char* dest, const char* src) 
{
    char* ret = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return ret;
}
