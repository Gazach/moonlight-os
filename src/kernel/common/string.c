// this code mostly came from (https://git.musl-libc.org/cgit/musl/tree/src/string)
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

// compare two strings, return 0 if they are equal, negative if s1 < s2, positive if s1 > s2
int strcmp(const char* s1, const char* s2) 
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// concatenate src to the end of dest
char* strcat(char* dest, const char* src) 
{
    char* ret = dest;
    while (*dest)
        dest++;
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
    return ret;
}

// memory related functions

// set the first n bytes of the block of memory pointed by s to the specified value c
void* memset(void* s, int c, size_t n) 
{
    unsigned char* p = s;
    while (n--)
        *p++ = (unsigned char)c;
    return s;
}

// copy n bytes from memory area src to memory area dest
void* memcpy(void* dest, const void* src, size_t n) 
{
    char* d = dest;
    const char* s = src;
    while (n--)
        *d++ = *s++;
    return dest;
}

// compare the first n bytes of memory area s1 and memory area s2
int memcmp(const void* s1, const void* s2, size_t n) 
{
    const unsigned char* p1 = s1;
    const unsigned char* p2 = s2;
    while (n--) {
        if (*p1 != *p2)
            return *p1 - *p2;
        p1++;
        p2++;
    }
    return 0;
}