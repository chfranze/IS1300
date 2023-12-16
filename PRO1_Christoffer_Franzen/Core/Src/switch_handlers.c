/**
******************************************************************************
@brief Switch handler
@file switch_handlers.c
@author Christoffer Franzén
@version 1.0
@date 19-November-2023
@brief
******************************************************************************
*/

#include <stdbool.h>

#include "usart.h"
#include "gpio.h"
#include "utilities.h"

/**
@brief readSwitch, reads the state of a switch connected to a specified GPIO pin.
@param GPIOx The GPIO port to which the switch is connected.
@param GPIO_Pin The GPIO pin to which the switch is connected.
@return bool Returns true if the switch is pressed (GPIO pin is low), false otherwise.
*/
bool readSwitch(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 0;
}

/**
@brief SW1_switch, reads the state of switch SW1.
@return bool Returns true if SW1 is pressed, false otherwise.
*/
bool SW1_switch(void){ return readSwitch(SW1_GPIO_Port, SW1_Pin); }

/**
@brief SW2_switch, reads the state of switch SW2.
@return bool Returns true if SW2 is pressed, false otherwise.
*/
bool SW2_switch(void){ return readSwitch(SW2_GPIO_Port, SW2_Pin); }

/**
@brief SW3_switch, reads the state of switch SW3.
@return bool Returns true if SW3 is pressed, false otherwise.
*/
bool SW3_switch(void){ return readSwitch(SW3_GPIO_Port, SW3_Pin); }

/**
@brief SW4_switch, reads the state of switch SW4.
@return bool Returns true if SW4 is pressed, false otherwise.
*/
bool SW4_switch(void){ return readSwitch(SW4_GPIO_Port, SW4_Pin); }

/**
@brief SW56_hit, reads the state of switches SW5 and SW6.
@return bool Returns true if either SW5 or SW6 is pressed, false otherwise.
*/
bool SW56_hit(void){ return readSwitch(SW56_GPIO_Port, SW56_Pin); }

/**
@brief SW78_hit, reads the state of switches SW7 and SW8.
@return bool Returns true if either SW7 or SW8 is pressed, false otherwise.
*/
bool SW78_hit(void){ return readSwitch(SW78_GPIO_Port, SW78_Pin); }

/**
 * @brief Checks if a switch is active.
 * @param GPIOx The GPIO port of the switch.
 * @param GPIO_Pin The GPIO pin of the switch.
 * @return Returns true if the switch is active (GPIO_PIN_SET), false otherwise.
 */
bool isSwitchActive(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET;
}

/**
 * @brief Checks if a switch is deactive.
 * @param GPIOx The GPIO port of the switch.
 * @param GPIO_Pin The GPIO pin of the switch.
 * @return Returns true if the switch is deactive (GPIO_PIN_RESET), false otherwise.
 */
bool isSwitchDeactive(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET;
}

