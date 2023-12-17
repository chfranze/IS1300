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

#include "FreeRTOS.h"

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

extern const TickType_t pedestrianDelay;
extern const TickType_t toggleFreq;
extern const TickType_t orangeDelay;
extern const TickType_t walkingDelay;
extern const TickType_t greenDelay;
extern const TickType_t redDelayMax;

/** @enum TrafficLightState
 *  @brief This enum represents the state of a traffic light.
 *  @var TrafficLightState::RED
 *  Member 'RED' represents the red state of the traffic light.
 *  @var TrafficLightState::ORANGE
 *  Member 'ORANGE' represents the orange state of the traffic light.
 *  @var TrafficLightState::GREEN
 *  Member 'GREEN' represents the green state of the traffic light.
 */
typedef enum {
    RED,
    ORANGE,
    GREEN
} TrafficLightState;

/** @struct TrafficLight
 *  @brief This structure represents a traffic light system for a 4-way intersection with pedestrian crossings.
 *  @var TrafficLight::north
 *  Member 'north' represents the state of the north traffic light.
 *  @var TrafficLight::south
 *  Member 'south' represents the state of the south traffic light.
 *  @var TrafficLight::east
 *  Member 'east' represents the state of the east traffic light.
 *  @var TrafficLight::west
 *  Member 'west' represents the state of the west traffic light.
 *  @var TrafficLight::pedestrianNS
 *  Member 'pedestrianNS' represents the state of the north-south pedestrian crossing.
 *  @var TrafficLight::pedestrianEW
 *  Member 'pedestrianEW' represents the state of the east-west pedestrian crossing.
 *  @var TrafficLight::button_pressed_NS
 *  Member 'button_pressed_NS' indicates whether the button for the north-south pedestrian crossing has been pressed.
 *  @var TrafficLight::button_pressed_EW
 *  Member 'button_pressed_EW' indicates whether the button for the east-west pedestrian crossing has been pressed.
 *  @var TrafficLight::car_present[4]
 *  Member 'car_present' is an array that holds the presence of cars for each direction.
 */
typedef struct {
    TrafficLightState north;
    TrafficLightState south;
    TrafficLightState east;
    TrafficLightState west;
    TrafficLightState pedestrianNS;
    TrafficLightState pedestrianEW;
    bool button_pressed_NS;
    bool button_pressed_EW;
    bool car_present[4];
} TrafficLight;


void TrafficLightStateMachine(TrafficLight *tl);
void initialize(TrafficLight *tl);
void updateInputs(TrafficLight *tl);
void toggleLED(int i, int led);

#endif /* INC_TRAFFIC_LIGHT_H_ */
