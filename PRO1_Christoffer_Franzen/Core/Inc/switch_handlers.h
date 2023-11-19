/**
******************************************************************************
@brief Switch handler
@file switch_handlers.h
@author Christoffer Franzén
@version 1.0
@date 19-November-2023
@brief Header file that contains definitions related to the operations of the switches and buttons
******************************************************************************
*/

#ifndef INC_SWITCH_HANDLERS_H_
#define INC_SWITCH_HANDLERS_H_

bool readSwitch(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
bool SW1_switch(void);
bool SW2_switch(void);
bool SW3_switch(void);
bool SW4_switch(void);
bool SW56_hit(void);
bool SW78_hit(void);

#endif /* INC_SWITCH_HANDLERS_H_ */
