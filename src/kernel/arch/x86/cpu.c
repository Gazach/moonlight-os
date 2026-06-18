#include "cpu.h"

void cpu_get_brand(char brand[CPU_BRAND_STRING_LENGTH])
{
    uint32_t* p = (uint32_t*)brand;

    for (uint32_t i = 0; i < 3; i++)
    {
        uint32_t eax, ebx, ecx, edx;

        __asm__ volatile (
            "cpuid"
            : "=a"(eax),
              "=b"(ebx),
              "=c"(ecx),
              "=d"(edx)
            : "a"(0x80000002 + i)
        );

        *p++ = eax;
        *p++ = ebx;
        *p++ = ecx;
        *p++ = edx;
    }

    brand[CPU_BRAND_STRING_LENGTH - 1] = '\0';
}

void cpu_get_vendor(char vendor[CPU_VENDOR_STRING_LENGTH])
{
    uint32_t ebx, ecx, edx;

    __asm__ volatile (
        "cpuid"
        : "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0)
    );

    ((uint32_t*)vendor)[0] = ebx;
    ((uint32_t*)vendor)[1] = edx;
    ((uint32_t*)vendor)[2] = ecx;

    vendor[CPU_VENDOR_STRING_LENGTH - 1] = '\0';
}

int cpu_is_x86_64(void)
{
    uint32_t eax, edx;

    __asm__ volatile(
        "cpuid"
        : "=a"(eax), "=d"(edx)
        : "a"(0x80000001)
        : "ebx", "ecx"
    );

    return (edx & (1 << 29)) != 0;
}