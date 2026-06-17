#ifndef PS2_H
#define PS2_H

#include <stdint.h>

// control ports
#define PS2_DATA_PORT 0x60
#define PS2_STATUS_REGISTER 0x64
#define PS2_COMMAND_REGISTER 0x64

// status register flags
#define PS2_STATUS_OUTPUT_BUFFER_FULL (1 << 0)
#define PS2_STATUS_INPUT_BUFFER_FULL (1 << 1)
#define PS2_STATUS_SYSTEM_FLAG (1 << 2)
#define PS2_STATUS_COMMAND_DATA (1 << 3)
#define PS2_STATUS_TIMEOUT_ERROR (1 << 6  )
#define PS2_STATUS_PARITY_ERROR (1 << 7)

// control commands
#define PS2_ENABLE_FIRST_PORT 0xAE
#define PS2_ENABLE_SECOND_PORT 0xA8
#define PS2_DISABLE_FIRST_PORT 0xAD
#define PS2_DISABLE_SECOND_PORT 0xA7
#define PS2_READ_CONTROLLER_CONFIG 0x20
#define PS2_WRITE_CONTROLLER_CONFIG 0x60
#define PS2_SELF_TEST 0xAA
#define PS2_TEST_FIRST_PORT 0xAB
#define PS2_TEST_SECOND_PORT 0xA9

void PS2_init();
uint8_t PS2_Status();
uint8_t PS2_ReadData();
void PS2_WriteData(uint8_t data);
void PS2_WriteCommand(uint8_t command);

#endif // PS2_H