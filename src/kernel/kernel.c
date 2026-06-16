

//kernel header
#include "terminal.h"

#include "std/printf.h"
// #include "common/string.h"


/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Test 1: basic string + newline */
	printf("Moonlight OS booted.\n");

	/* Test 2: multiple newlines / line separation */
	printf("Line 1\nLine 2\nLine 3\n");

	/* Test 3: integer formatting */
	printf("Decimal: %d\n", 1337);
	printf("Hex:     %x\n", 0xDEAD);
	printf("Unsigned:%u\n", 4294967295u);

	/* Test 4: string argument */
	printf("String:  %s\n", "hello from printf");

	/* Test 5: mixed */
	printf("Kernel addr base: 0x%x, stack size: %d KB\n", 0x100000, 16);

	printf("--- all tests done ---\n");

	/* Test 6: strlen */
	const char* s = "Moonlight";
	printf("strlen(\"Moonlight\") = %d\n", (int)strlen(s));

	/* Test 7: strcpy */
	char buf[32];
	strcpy(buf, "copied!");
	printf("strcpy: %s\n", buf);

	/* Test 8: strcmp */
	printf("strcmp(\"abc\",\"abc\") = %d\n", strcmp("abc", "abc"));
	printf("strcmp(\"abc\",\"xyz\") = %d\n", strcmp("abc", "xyz"));

	/* Test 9: strcat */
	strcpy(buf, "hello ");
	strcat(buf, "world");
	printf("strcat: %s\n", buf);

	/* Test 10: memset */
	char mbuf[8];
	memset(mbuf, 'A', 7);
	mbuf[7] = '\0';
	printf("memset: %s\n", mbuf);

	/* Test 11: memcpy */
	char dst[10];
	memcpy(dst, "memcpy!\0", 8);
	printf("memcpy: %s\n", dst);

	/* Test 12: memcmp */
	printf("memcmp(\"abc\",\"abc\",3) = %d\n", memcmp("abc", "abc", 3));
	printf("memcmp(\"abc\",\"abd\",3) = %d\n", memcmp("abc", "abd", 3));

	printf("--- string tests done ---\n");
}