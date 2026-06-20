#include "isr.h"
#include "irq.h"
#include "../../../std/printf.h"
#include "../../../terminal.h"

// exception names for vectors 0-31
static const char* exception_names[] = {
    "Divide By Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point",
    "Virtualization",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved"
};

// Dump the register values from the interrupt frame for debugging purposes
void dump_registers(interrupt_frame_t* frame) {
    printf("\n=== REGISTER DUMP ===\n");
    printf("EAX=%08x  EBX=%08x  ECX=%08x  EDX=%08x\n",
        frame->eax, frame->ebx, frame->ecx, frame->edx);
    printf("ESI=%08x  EDI=%08x  EBP=%08x  ESP=%08x\n",
        frame->esi, frame->edi, frame->ebp, frame->esp);
    printf("EIP=%08x  CS =%08x  EFL=%08x\n",
        frame->eip, frame->cs, frame->eflags);
    printf("DS =%08x  ES =%08x  FS =%08x  GS =%08x\n",
        frame->ds, frame->es, frame->fs, frame->gs);
    printf("INT=%u  ERR=%08x\n", frame->int_no, frame->err_code);
    printf("=====================\n");
}

// ISR handler for CPU exceptions
void isr_handler(interrupt_frame_t* frame) {
    if (frame->int_no >= 32 && frame->int_no <= 47) {
        // hardware IRQ — call irq_handler
        uint8_t irq = frame->int_no - 32;
        irq_handler(irq);
        return;
    }

    if (frame->int_no >= 32) return;  // unknown vector

    // breakpoint — recoverable
    if (frame->int_no == 3) {
        printf("\nBreakpoint hit:\n");
        dump_registers(frame);
        return;
    }

    // fatal exception
    printf("\n--- KERNEL EXCEPTION ---\n");
    printf("Exception : %s (#%u)\n",
        exception_names[frame->int_no], frame->int_no);
    dump_registers(frame);

    if (frame->int_no == 14) {
        uint32_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        printf("CR2 (fault addr): 0x%x\n", cr2);
        printf("Cause: %s %s %s\n",
            (frame->err_code & 1) ? "protection" : "not present",
            (frame->err_code & 2) ? "write"      : "read",
            (frame->err_code & 4) ? "(user)"     : "(kernel)");
    }

    printf("System halted.\n");
    __asm__ volatile("cli; hlt");
}