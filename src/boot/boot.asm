; boot.asm
section .multiboot_header
align 8

header_start:
    dd 0xE85250D6                          ; magic
    dd 0                                   ; arch
    dd header_end - header_start           ; header length
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))  ; checksum

    ; Required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .bss
    align 16
stack_bottom:
    resb 16384           ; 16KB stack
stack_top:

section .text
extern kernel_main
global _start
_start:
    cli
    mov esp, stack_top
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang