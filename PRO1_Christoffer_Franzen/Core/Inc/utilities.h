/**
******************************************************************************
@brief Utilities, program for controlling lights and more
@file utilities.h
@author Christoffer Franzén
@version 1.0
@date 18-November-2023
@brief Contain definitions related to the lights and more
******************************************************************************
*/

#ifndef INC_UTILITIES_H_
#define INC_UTILITIES_H_

#define LED1 0x000001	// TL1_Yellow
#define LED2 0x000002	// TL1_Green
#define LED3 0x000004	// PL1_Red
#define LED4 0x000010
#define LED5 0x000020
#define LED6 0x
#define LED7 0x
#define LED8 0x200
#define LED9 0x400
#define LED10 0x800
#define LED11 0x1000
#define LED12 0x200000
#define LED13 0x00000D
#define LED14 0x
#define LED15 0x00000F
#define LED16 0x
#define LED17 0x
#define LED18 0x
#define LED19 0x
#define LED20 0x
#define LED21 0x
#define LED22 0x
#define LED23 0x
#define LED24 0x100000


void sendSPIdata(uint8_t data[], uint16_t byteSize);
void lightUpLEDs(uint32_t leds[], uint16_t numLeds);

#endif /* INC_UTILITIES_H_ */
