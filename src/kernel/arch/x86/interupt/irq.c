#include "irq.h"
#include "pic.h"
#include "../../../terminal.h"

// function pointer type for IRQ handlers
typedef void (*irq_handler_t)(void);

// table of handlers — one per IRQ
static irq_handler_t irq_handlers[16] = {0};

void irq_install(uint8_t irq, irq_handler_t handler) {
    irq_handlers[irq] = handler;
}

void irq_handler(uint32_t irq) {
    // call the registered handler if there is one
    if (irq_handlers[irq])
        irq_handlers[irq]();

    // always send EOI so PIC knows we're done
    pic_send_eoi(irq);
}

void irq_init(void) {
    // nothing here yet — handlers registered individually
}