#include "ad5272.h"
#include "gpio.h"
#include "i2c.h"
#include "SEGGER_RTT.h"

void InitDigipot() {
  	// Reset digipot
    HAL_GPIO_WritePin(POT_RST_GPIO_Port, POT_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

    AD5272_command_write(&hi2c1, AD5272_ADDRESS, 0x00, 0x0000);
    HAL_Delay(200);

    AD5272_control_write_verified(&hi2c1, AD5272_ADDRESS, AD5272_RDAC_WIPER_WRITE_ENABLE);
    HAL_Delay(200);
}

int8_t AD5272_command_write(I2C_HandleTypeDef *bus, uint8_t address, uint8_t command, uint16_t write_datum16)
{
    uint8_t error_count = 0;
    uint8_t data[2];

    int8_t return_val = 0;

    if (write_datum16 > 0x3FF)
    {
        // data in bits 13:10 will clobber the command when we OR in write_datum16
        write_datum16 &= 0x3FF; // clip off any bad high bits even though we are going to error out and not use them
        // this is an error in the code using this function, we should not proceed with the write
        error_count |= 0x10;
    }

    if ((AD5272_RDAC_WRITE == command) || (AD5272_CONTROL_WRITE == command) || (AD5272_SHUTDOWN == command))
    {
        // these commands need to use data we send over
        // shift the command over into bits 13:10
        data[0] = (command << 2) | (write_datum16 >> 8);
        // also need to send 10-bit or 8-bit wiper value, or 3 control bits, or shutdown bit
        data[1] = write_datum16 & 0xFF;
    }
    else if ((AD5272_50TP_WRITE == command) || (AD5272_RDAC_REFRESH == command) || (AD5272_COMMAND_NOP == command))
    {
        // shift the command over into bits 13:10
        data[0] = command << 2;
        data[1] = 0;
        // no data needed
    }
    else
    {
        // It's either a bad command (out of range, > AD5272_SHUTDOWN), or its not a writeable command
        // Bad command, we can't reasonably proceed
        error_count |= 0x20;
    }

    // if no errors so far, command is valid and datum is too
    if (0 == error_count)
    {
        // data = (data_to_write >> 8);		// ms byte to write
        // count = Wire.write(data);
        // data = data_to_write & 0x0FF;		// ls byte to write
        // count += Wire.write(data);
        if (HAL_I2C_Master_Transmit(bus, address, data, 2, 0x1000) != HAL_OK)
            error_count += 2; // add number of bad writes
    }

    // if errors, return the count, make it negative first
    if (error_count > 0)
    {
        return_val = -1 * error_count;
    }

    return return_val;
}

int8_t AD5272_control_write_verified(I2C_HandleTypeDef *bus, uint8_t address, uint8_t control)
{
    uint8_t error_count = 0;
    uint8_t data[2];

    if (control > 7) // bad value, what is caller's intent?
    {
        // AD5272 will ignore all bits except 2:0 so we can proceed
        // but this is still not good, so bump the error counter
        error_count++;
    }

    data[0] = AD5272_CONTROL_WRITE << 2;
    data[1] = control;

    if (HAL_I2C_Master_Transmit(bus, address, data, 2, 0x1000) != HAL_OK)
    {
        error_count += 2; // add number of bad writes
    }

    // now see if control reg was correctly written by reading it

    // write the control read command, data except the command value are don't cares
    data[0] = AD5272_CONTROL_READ << 2;
    data[1] = 0;

    if (HAL_I2C_Master_Transmit(bus, address, data, 2, 0x1000) != HAL_OK)
    {
        error_count += 2; // add number of bad writes
    }

    data[0] = 0xFF;
    data[1] = 0xFF;
    // now request the read data from the control register
    if (HAL_I2C_Master_Receive(bus, address, data, 2, 0x1000) != HAL_OK)
    {
        SEGGER_RTT_printf(0, "AD5272 failed to read\n");
        error_count += 2;
    }

    if (data[1] != control)
    {
        SEGGER_RTT_printf(0, "AD5272 control got %x\n", data[1]);
        SEGGER_RTT_printf(0, "AD5272 control want %x\n", control);
        return -1 * error_count;
    }

    return error_count;
}
