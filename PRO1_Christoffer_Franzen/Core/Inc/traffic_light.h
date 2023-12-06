/**
******************************************************************************
@brief Traffic light, program for controlling traffic light
@file traffic_light.h
@author Christoffer Franzén
@version 1.0
@date 18-November-2023
@brief Header file that contains definitions related to the operations of the traffic light
******************************************************************************
*/

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

extern uint8_t toggleFreq;       // Frequency of toggle in ms
extern uint8_t pedestrianDelay;  // Delay in ms after the pedestrian pushed the button
extern uint8_t walkingDelay;     // Time in ms the pedestrian signal stays green
extern uint8_t orangeDelay;      // Time in ms a car signal remains orange

void TrafficLightInitialization(void);
void TrafficLightStateMachine(void);

#endif /* INC_TRAFFIC_LIGHT_H_ */
