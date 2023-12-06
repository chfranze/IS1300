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

uint8_t toggleFreq = 10;
uint8_t pedestrianDelay = 10;
uint8_t walkingDelay = 10;
uint8_t orangeDelay = 10;

/**
@brief
@param
@param
@return void
*/
void TrafficLightInitialization(void)
{
	int InitializeLEDs[] = {19, 20, 6 , 12};

	for(int i = 0; i < 4; i++)
	{
		ledOn(InitializeLEDs[i]);
	}
}

/**
@brief
@param
@param
@return void
*/
void TrafficLightStateMachine(void)
{
	while(1)
	{


	}
}

