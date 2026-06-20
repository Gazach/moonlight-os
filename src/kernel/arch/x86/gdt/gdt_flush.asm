global gdt_flush

gdt_flush:
    mov eax, [esp + 4]
    lgdt [eax]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push dword 0x08     ; selector
    push dword .flush   ; offset
    retf                ; far return = far jump
.flush:
    ret