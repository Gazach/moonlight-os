#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
int strcmp(const char* s1, const char* s2);
char* strcat(char* dest, const char* src);
void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

#endif /* STRING_H */