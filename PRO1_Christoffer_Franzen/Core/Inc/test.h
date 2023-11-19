/**
******************************************************************************
@brief Test, test program for project
@file test.h
@author Christoffer Franzén
@version 1.0
@date 18-November-2023
@brief Header for test program
******************************************************************************
*/

#ifndef INC_TEST_H_
#define INC_TEST_H_

void Test_program(void);
void Test_Led(void);
void Test_Lights2(void);
void sendSPIdata(uint8_t data[], uint16_t byteSize);
void Test_TraficLights(void);
void lightUpLED1(void);
void lightUpLED(uint8_t leds[][3], size_t num_leds);
void Test_lightUpLEDs(void);

#endif /* INC_TEST_H_ */
