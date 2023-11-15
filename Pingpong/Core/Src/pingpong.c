/**
 ******************************************************************************
 @brief Pingpong statemachine for Pingpong-program
 @file pingpong.c
 @author Bengt Molin
 @version 1.0
 @date 12-August-2019
 @brief Functions and structures for program Pingpong
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "pingpong.h"
/* Define states for state machine*/

typedef enum {
	Start, MoveRight, MoveLeft
} states;

static states State, NextState;

/**
@brief Pingpong, main program for the pingpong game
*/
void Pingpong(void) {
	bool ButtonPressed = false; // To remember that button is pressed
	uint32_t Varv;
	uint32_t Speed; // Ball speed
	uint8_t Led; // LED nr
	uint8_t L_points;
	uint8_t R_points;
	State = Start; // Initiate State to Start
	NextState = Start;
	Speed = 500000; // Number of loops
	R_points = 0;
	L_points = 0;
	bool L_Serve = true;
	bool R_Serve = true;


	/* Infinite loop */
	while (1) {
		State = NextState;
		switch (State) // State machine
		{
		case Start:
			Led_on(0); // Turn off all LEDs

			if (L_hit() == true && L_Serve) // L serve
			{
				L_Serve = false;
				R_Serve = true;
				Led = 1;
				NextState = MoveRight;
				while (L_hit() == true)
					; // wait until button is released
			} else if (R_hit() == true && R_Serve) // R serve
			{
				L_Serve = true;
				R_Serve = false;
				Led = 8;
				NextState = MoveLeft;
				while (R_hit() == true)
					; // wait until button is released
			} else
				NextState = Start; // Stay in Start state
			break;

		case MoveRight: {
			Led_on(Led);
			Varv = Speed;
			while (Varv != 0) {
				if (R_hit())
					ButtonPressed = true; // R hit
				Varv--;
			}
			if (ButtonPressed) // R pressed
			{
				if (Led == 8) // and LED8 activa
						{
					NextState = MoveLeft; // return ball
					Led = 7;
				} else {
					L_points++;
					Point_Handler(&L_points, &R_points, &Speed, &L_Serve, &R_Serve);
					NextState = Start; // hit to early
				}
			} else {
				if (Led == 9) // no hit or to late
						{
					Show_points(4, 4);
					HAL_Delay(500);
					L_points++;
					Point_Handler(&L_points, &R_points, &Speed, &L_Serve, &R_Serve);
					NextState = Start;
				} else
					NextState = MoveRight; // ball continues to move right
			}
			if (!ButtonPressed)
				Led++; // prepare to turn next LED on
			ButtonPressed = false;
		}
			break;
		case MoveLeft: {
			Led_on(Led);
			Varv = Speed;
			while (Varv != 0) {
				if (L_hit())
					ButtonPressed = true; // L hit
				Varv--;
			}
			if (ButtonPressed) // L pressed
			{
				if (Led == 1) // and LED1 active
						{
					NextState = MoveRight; // return ball
					Led = 2;
				} else {
					R_points++;
					Point_Handler(&L_points, &R_points, &Speed, &L_Serve, &R_Serve);
					NextState = Start; // hit to early
				}
			} else {
				if (Led == 0) // no hit or to late
						{
					Show_points(4, 4);
					HAL_Delay(500);
					R_points++;
					Point_Handler(&L_points, &R_points, &Speed, &L_Serve, &R_Serve);
					NextState = Start;
				} else
					NextState = MoveLeft; // ball continues to move left
			}
			if (!ButtonPressed)
				Led--; // prepare to turn next LED on
			ButtonPressed = false;
		}
			break;
		default:
			break;
		}
	}
} // End of function Pingpong
