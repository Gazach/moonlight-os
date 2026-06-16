/*
 * 64-bit division/modulo helpers for i386 freestanding builds.
 * The compiler emits calls to __udivdi3/__umoddi3 when it encounters
 * 64-bit division and no runtime library is linked (-nostdlib).
 */

#include <stdint.h>

uint64_t __udivdi3(uint64_t a, uint64_t b)
{
	if (b == 0)
		return 0;

	uint64_t quotient  = 0;
	uint64_t remainder = 0;

	for (int i = 63; i >= 0; i--) {
		remainder = (remainder << 1) | ((a >> i) & 1);
		if (remainder >= b) {
			remainder -= b;
			quotient |= (uint64_t)1 << i;
		}
	}
	return quotient;
}

uint64_t __umoddi3(uint64_t a, uint64_t b)
{
	if (b == 0)
		return 0;
	return a - b * __udivdi3(a, b);
}
