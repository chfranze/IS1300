/**
 ******************************************************************************
 * @brief Global variable declarations
 * @file global.h
 * @author Christoffer Franzén
 * @version 1.0
 * @date 2023-12-17
 * @details This file contains the declarations of global variables used across
 * different files in the project. It includes the FreeRTOS and CMSIS OS headers,
 * and declares several variables related to LED data and task handling.
 ******************************************************************************
 */

// FreeRTOS and CMSIS OS headers
#include "FreeRTOS.h"
#include "cmsis_os.h"

// Array to hold the current LED data
extern uint8_t data[3];

// Array to hold the last LED data
extern uint8_t lastData[3];

// 2D array to hold the data for multiple LEDs
extern uint8_t LED_arr[24][3];

// Handle for the ToggleLED task
extern osThreadId_t ToggleLEDHandle;

// Variable to hold the ID of the current LED
extern uint8_t ledID;
