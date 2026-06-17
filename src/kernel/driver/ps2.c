#include "ps2.h"
#include "../arch/io.h"

//check the status of the PS/2 controller
uint8_t PS2_Status() {
    return inb(PS2_STATUS_REGISTER);
}

// waiting for the output buffer to be full (data ready to read)
static void PS2_WaitOutput() {
    while (!(PS2_Status() & PS2_STATUS_OUTPUT_BUFFER_FULL));
}

// waiting for the input buffer to be empty (ready to accept new data)
static void PS2_WaitInput() {
    while (PS2_Status() & PS2_STATUS_INPUT_BUFFER_FULL);
}

// read a byte of data from the PS/2 data port
uint8_t PS2_ReadData() {
    PS2_WaitOutput();
    return inb(PS2_DATA_PORT);
}

// write a byte of data to the PS/2 data port
void PS2_WriteData(uint8_t data) {
    PS2_WaitInput();
    outb(PS2_DATA_PORT, data);
}

void PS2_WriteCommand(uint8_t command) {
    PS2_WaitInput();
    outb(PS2_COMMAND_REGISTER, command);
}

void PS2_init(){
    // Disable devices
    PS2_WriteCommand(PS2_DISABLE_FIRST_PORT);
    PS2_WriteCommand(PS2_DISABLE_SECOND_PORT);

    // Flush output buffer
    while (PS2_Status() & PS2_STATUS_OUTPUT_BUFFER_FULL) {
        inb(PS2_DATA_PORT);
    }

    // Set controller configuration: enable interrupts for first port, disable second port
    PS2_WriteCommand(PS2_READ_CONTROLLER_CONFIG);
    uint8_t config = PS2_ReadData();
    config &= ~(1 << 0); // Disable first port interrupt
    config &= ~(1 << 1); // Disable second port interrupt
    PS2_WriteCommand(PS2_WRITE_CONTROLLER_CONFIG);
    PS2_WriteData(config);

    // Perform self-test
    PS2_WriteCommand(PS2_SELF_TEST);
    if (PS2_ReadData() != 0x55) {
        // Handle self-test failure (e.g., log error, halt system)
        return;
    }

    // Enable first port (keyboard)
    PS2_WriteCommand(PS2_ENABLE_FIRST_PORT);

    // Enable first port IRQ in controller config
    PS2_WriteCommand(PS2_READ_CONTROLLER_CONFIG);
    config = PS2_ReadData();
    config |= (1 << 0); // Enable first port interrupt
    PS2_WriteCommand(PS2_WRITE_CONTROLLER_CONFIG);
    PS2_WriteData(config);

}