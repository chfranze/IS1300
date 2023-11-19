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

uint8_t data[3] = {0x00, 0x00, 0x00};

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
    HAL_Delay(1);
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_SET);

    // Disable the output
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_SET);

    // Transmit data
    HAL_SPI_Transmit(&hspi3, &data[0], byteSize, 10);

    // Trigger registers
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_RESET);

    // Enable output
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_RESET);
}

/**
@brief lightUpLEDs, uses the sendSPIdata function to send data to a series of LEDs connected to a 74HC595D shift register.
@param leds[][3] A two-dimensional array where each row corresponds to an LED and each column corresponds to a byte of the LED's data.
@param num_leds The number of LEDs in the leds array.
@return void
*/
void lightUpLEDs(uint8_t leds[][3], size_t num_leds) {
    for (size_t i = 0; i < num_leds; i++) {
        data[0] |= leds[i][0];
        data[1] |= leds[i][1];
        data[2] |= leds[i][2];
    }

    uint16_t byteSize = sizeof(data);
    sendSPIdata(data, byteSize);
}

void Led_on(uint8_t Lednr)
{
    uint8_t i;

    uint8_t LED1_arr[1][3] = {LED1};
    uint8_t LED2_arr[1][3] = {LED2};
    uint8_t LED3_arr[1][3] = {LED3};
    uint8_t LED4_arr[1][3] = {LED4};
    uint8_t LED5_arr[1][3] = {LED5};
    uint8_t LED6_arr[1][3] = {LED6};
    uint8_t LED7_arr[1][3] = {LED7};
    uint8_t LED8_arr[1][3] = {LED8};
    uint8_t LED9_arr[1][3] = {LED9};
    uint8_t LED10_arr[1][3] = {LED10};
    uint8_t LED11_arr[1][3] = {LED11};
    uint8_t LED12_arr[1][3] = {LED12};
    uint8_t LED13_arr[1][3] = {LED13};
    uint8_t LED14_arr[1][3] = {LED14};
    uint8_t LED15_arr[1][3] = {LED15};
    uint8_t LED16_arr[1][3] = {LED16};
    uint8_t LED17_arr[1][3] = {LED17};
    uint8_t LED18_arr[1][3] = {LED18};
    uint8_t LED19_arr[1][3] = {LED19};
    uint8_t LED20_arr[1][3] = {LED20};
    uint8_t LED21_arr[1][3] = {LED21};
    uint8_t LED22_arr[1][3] = {LED22};
    uint8_t LED23_arr[1][3] = {LED23};
    uint8_t LED24_arr[1][3] = {LED24};

    switch(Lednr){
    case 1:
        lightUpLEDs(LED1_arr, 1);
        break;
    case 2:
        lightUpLEDs(LED2_arr, 1);
        break;
    case 3:
        lightUpLEDs(LED3_arr, 1);
        break;
    case 4:
        lightUpLEDs(LED4_arr, 1);
        break;
    case 5:
        lightUpLEDs(LED5_arr, 1);
        break;
    case 6:
        lightUpLEDs(LED6_arr, 1);
        break;
    case 7:
        lightUpLEDs(LED7_arr, 1);
        break;
    case 8:
        lightUpLEDs(LED8_arr, 1);
        break;
    case 9:
        lightUpLEDs(LED9_arr, 1);
        break;
    case 10:
        lightUpLEDs(LED10_arr, 1);
        break;
    case 11:
        lightUpLEDs(LED11_arr, 1);
        break;
    case 12:
        lightUpLEDs(LED12_arr, 1);
        break;
    case 13:
        lightUpLEDs(LED13_arr, 1);
        break;
    case 14:
        lightUpLEDs(LED14_arr, 1);
        break;
    case 15:
        lightUpLEDs(LED15_arr, 1);
        break;
    case 16:
        lightUpLEDs(LED16_arr, 1);
        break;
    case 17:
        lightUpLEDs(LED17_arr, 1);
        break;
    case 18:
        lightUpLEDs(LED18_arr, 1);
        break;
    case 19:
        lightUpLEDs(LED19_arr, 1);
        break;
    case 20:
        lightUpLEDs(LED20_arr, 1);
        break;
    case 21:
        lightUpLEDs(LED21_arr, 1);
        break;
    case 22:
        lightUpLEDs(LED22_arr, 1);
        break;
    case 23:
        lightUpLEDs(LED23_arr, 1);
        break;
    case 24:
        lightUpLEDs(LED24_arr, 1);
        break;
    default:
    	break;
    }
}

void led_off(uint8_t Lednr)
{
    uint8_t LED1_arr[1][3] = {LED1};
    uint8_t LED2_arr[1][3] = {LED2};
    uint8_t LED3_arr[1][3] = {LED3};
    uint8_t LED4_arr[1][3] = {LED4};
    uint8_t LED5_arr[1][3] = {LED5};
    uint8_t LED6_arr[1][3] = {LED6};
    uint8_t LED7_arr[1][3] = {LED7};
    uint8_t LED8_arr[1][3] = {LED8};
    uint8_t LED9_arr[1][3] = {LED9};
    uint8_t LED10_arr[1][3] = {LED10};
    uint8_t LED11_arr[1][3] = {LED11};
    uint8_t LED12_arr[1][3] = {LED12};
    uint8_t LED13_arr[1][3] = {LED13};
    uint8_t LED14_arr[1][3] = {LED14};
    uint8_t LED15_arr[1][3] = {LED15};
    uint8_t LED16_arr[1][3] = {LED16};
    uint8_t LED17_arr[1][3] = {LED17};
    uint8_t LED18_arr[1][3] = {LED18};
    uint8_t LED19_arr[1][3] = {LED19};
    uint8_t LED20_arr[1][3] = {LED20};
    uint8_t LED21_arr[1][3] = {LED21};
    uint8_t LED22_arr[1][3] = {LED22};
    uint8_t LED23_arr[1][3] = {LED23};
    uint8_t LED24_arr[1][3] = {LED24};

    switch(Lednr){
    case 1:
        data[0] &= ~LED1_arr[0][0];
        data[1] &= ~LED1_arr[0][1];
        data[2] &= ~LED1_arr[0][2];
        break;
    case 2:
        data[0] &= ~LED2_arr[0][0];
        data[1] &= ~LED2_arr[0][1];
        data[2] &= ~LED2_arr[0][2];
        break;
    default:
    	break;
    }
    uint16_t byteSize = sizeof(data);
    sendSPIdata(data, byteSize);
}
