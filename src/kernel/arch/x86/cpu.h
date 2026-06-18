#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define CPU_BRAND_STRING_LENGTH 50
#define CPU_VENDOR_STRING_LENGTH 20

void cpu_get_brand(char brand[CPU_BRAND_STRING_LENGTH]);
void cpu_get_vendor(char vendor[CPU_VENDOR_STRING_LENGTH]);
int cpu_is_x86_64(void);

#endif // CPU_H