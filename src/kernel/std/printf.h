#ifndef __SIMPLE_PRINTF__
#define __SIMPLE_PRINTF__

/* Platform must supply this function to output a character */
int putchar(int c);

int simple_printf(const char *fmt, ...);
int simple_sprintf(char *buf, const char *fmt, ...);

#define printf  simple_printf
#define sprintf simple_sprintf

#endif /* __SIMPLE_PRINTF__ */