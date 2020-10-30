#ifndef __AD5272_H
#define __AD5272_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f0xx_hal.h"

/**
 * I2C ADDRESS CONSTANTS
 * I2C base addresses of AD5274 dependent on ADDR pin connection
*/
#define AD5272_BASE_ADDR_GND 0x2F   /**< Default I2C address iff ADDR=GND, see notes above */
#define AD5272_BASE_ADDR_VDD 0x2C   /**< I2C address iff ADDR=VDD, see notes above */
#define AD5272_BASE_ADDR_FLOAT 0x2E /**< I2C address iff ADDR=floating, see notes above */

/**
 * COMMAND CONSTANTS
 * See Table 12, page 21, in the Rev D datasheet
 * Commands are 16-bit writes: bits 15:14 are 0s
 * Bits 13:10 are the command value below
 * Bits 9:0 are data for the command, but not all bits are used with all commands
 */

// The NOP command is included for completeness. It is a valid I2C operation.
#define AD5272_COMMAND_NOP 0x00 // Do nothing. Why you would want to do this I don't know

// write the 10 or 8 data bits to the RDAC wiper register (it must be unlocked first)
#define AD5272_RDAC_WRITE 0x01

#define AD5272_RDAC_READ 0x02 // read the RDAC wiper register

#define AD5272_50TP_WRITE 0x03 // store RDAC setting to 50-TP

// SW reset: refresh RDAC with last 50-TP stored value
// If not 50-TP value, reset to 50% I think???
// data bits are all dont cares
#define AD5272_RDAC_REFRESH 0x04 // TODO refactor this to AD5272_SOFT_RESET

// read contents of 50-TP in next frame, at location in data bits 5:0,
// see Table 16 page 22 Rev D datasheet
// location 0x0 is reserved, 0x01 is first programmed wiper location, 0x32 is 50th programmed wiper location
#define AD5272_50TP_WIPER_READ 0x05

/**
 * Read contents of last-programmed 50-TP location
 * This is the location used in SW Reset command 4 or on POR
 */
#define AD5272_50TP_LAST_USED 0x06

#define AD5272_CONTROL_WRITE 0x07 // data bits 2:0 are the control bits

#define AD5272_CONTROL_READ 0x08 // data bits all dont cares

#define AD5272_SHUTDOWN 0x09 // data bit 0 set = shutdown, cleared = normal mode

/**
 * Control bits are three bits written with command 7
 */
// enable writing to the 50-TP memory by setting this control bit C0
// default is cleared so 50-TP writing is disabled
// only 50 total writes are possible!
#define AD5272_50TP_WRITE_ENABLE 0x01

// enable writing to volatile RADC wiper by setting this control bit C1
// otherwise it is frozen to the value in the 50-TP memory
// default is cleared, can't write to the wiper
#define AD5272_RDAC_WIPER_WRITE_ENABLE 0x02

// enable high precision calibration by clearing this control bit C2
// set this bit to disable high accuracy mode (dunno why you would want to)
// default is 0 = emabled
#define AD5272_RDAC_CALIB_DISABLE 0x04

// 50TP memory has been successfully programmed if this bit is set
#define AD5272_50TP_WRITE_SUCCESS 0x08

int8_t AD5272_command_write(I2C_HandleTypeDef *bus, uint8_t address, uint8_t command, uint16_t write_datum16);
int8_t AD5272_control_write_verified(I2C_HandleTypeDef *bus, uint8_t address, uint8_t control);

void InitDigipot();

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
