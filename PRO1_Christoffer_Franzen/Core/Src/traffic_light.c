/**
******************************************************************************
@brief Traffic light, program for controlling traffic light
@file traffic_light.c
@author Christoffer Franzén
@version 1.0
@date 18-November-2023
@brief Contain the functions and definitions related to the operations of the traffic light
******************************************************************************
*/


#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "utilities.h"

void Traffic_light(void)
{
    uint8_t data1[3] = {0xff, 0xff, 0xff};
    uint8_t data2[3] = {0x00, 0x00, 0x00};

    for (int i = 0; i < 30; i++)
    {
        sendSPIdata(&data1[0], 3);
        HAL_Delay(500);
        sendSPIdata(&data2[0], 3);
        HAL_Delay(500);
    }
}
