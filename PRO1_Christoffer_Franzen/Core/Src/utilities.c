/**
******************************************************************************
@brief Utilities, program for controlling lights and more
@file utilities.c
@author Christoffer Franzén
@version 1.0
@date 18-November-2023
@brief Contain the functions and definitions related to the lights and more
******************************************************************************
*/

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "utilities.h"
#include "spi.h"

/**
@brief sendSPIdata, uses the SPI3 bus to send data to a 74HC595D shift register, which is used to control traffic lights.
@param data[] An array of bytes to be sent via SPI.
@param byteSize The number of bytes in the data array to be sent.
@return void
*/
void sendSPIdata(uint8_t data[], uint16_t byteSize)
{
    // Clear the buffer
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);  // Wait for 1ms
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_SET);

    // Disable the output
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_SET);

    // Transmit data
    HAL_SPI_Transmit(&hspi3, &data[0], byteSize, 10);

    // Trigger registers
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_SET);
    HAL_Delay(1);  // Wait for 1ms
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_RESET);

    // Enable output
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_RESET);
}

void lightUpLEDs(uint32_t leds[], uint16_t numLeds)
{
    uint8_t data[3] = {0x00, 0x00, 0x00};  // Initialize data to 0

    // OR each LED value with data
    for(int i = 0; i < numLeds; i++) {
        data[0] |= (leds[i] >> 16) & 0xFF;
        data[1] |= (leds[i] >> 8) & 0xFF;
        data[2] |= leds[i] & 0xFF;
    }

    // Send data via SPI
    sendSPIdata(&data[0], 3);
}
