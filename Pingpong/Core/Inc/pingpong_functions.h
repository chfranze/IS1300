/**
******************************************************************************
@brief pingpong_functions, header for clock-program
@file clock.h
@author Christoffer Franzén
@version 1.0
@date 07-November-2019
@brief Functions and structures for program pingpong'
******************************************************************************
*/

#ifndef INC_PINGPONG_FUNCTIONS_H_
#define INC_PINGPONG_FUNCTIONS_H_

void Led_on(uint8_t Lednr);
void Set_led(uint8_t Lednr);
void Show_points(uint8_t L_points, uint8_t R_points);
bool L_hit(void);
bool R_hit(void);
void Point_Handler(uint8_t *L_Points, uint8_t *R_Points, uint32_t *Speed, bool *L_Serve, bool *R_Serve);

#endif /* INC_PINGPONG_FUNCTIONS_H_ */
