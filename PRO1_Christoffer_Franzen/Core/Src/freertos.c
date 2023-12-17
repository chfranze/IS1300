/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "test.h"
#include "utilities.h"
#include "traffic_light.h"
#include "global.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticMutexDef_t;
/* USER CODE BEGIN PTD */
osThreadId_t ToggleLEDHandle;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RUN_TEST_PROGRAM
#undef RUN_TEST_PROGRAM
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TrafficLight tl;
uint8_t ledID;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityNormal, };
/* Definitions for SendSPITask */
osThreadId_t SendSPITaskHandle;
const osThreadAttr_t SendSPITask_attributes = { .name = "SendSPITask",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityHigh, };
/* Definitions for TrafficLight */
osThreadId_t TrafficLightHandle;
const osThreadAttr_t TrafficLight_attributes = { .name = "TrafficLight",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityNormal, };
/* Definitions for ToggleLED */
osThreadId_t ToggleLEDHandle;
const osThreadAttr_t ToggleLED_attributes = { .name = "ToggleLED", .stack_size =
		128 * 4, .priority = (osPriority_t) osPriorityHigh, };
/* Definitions for spiMutex */
osMutexId_t spiMutexHandle;
osStaticMutexDef_t spiMutexControlBlock;
const osMutexAttr_t spiMutex_attributes = { .name = "spiMutex", .cb_mem =
		&spiMutexControlBlock, .cb_size = sizeof(spiMutexControlBlock), };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void wait_cycles(uint32_t n);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void SendSPIdataTask(void *argument);
void TrafficLightTask(void *argument);
void ToggleLEDTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */
	/* Create the mutex(es) */
	/* creation of spiMutex */
	spiMutexHandle = osMutexNew(&spiMutex_attributes);

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

	/* creation of SendSPITask */
	SendSPITaskHandle = osThreadNew(SendSPIdataTask, NULL,
			&SendSPITask_attributes);

	/* creation of TrafficLight */
	TrafficLightHandle = osThreadNew(TrafficLightTask, NULL,
			&TrafficLight_attributes);

	/* creation of ToggleLED */
	ToggleLEDHandle = osThreadNew(ToggleLEDTask, NULL, &ToggleLED_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	vTaskSuspend(ToggleLEDHandle);
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN StartDefaultTask */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10); // ms to ticks

	/* Infinite loop */
	xLastWakeTime = xTaskGetTickCount();

	for (;;) {
#ifdef RUN_TEST_PROGRAM
	  test_program();
	  vTaskDelayUntil(&xLastWakeTime, xPeriod);
	#else
		vTaskDelete(NULL);
#endif
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_SendSPIdataTask */
/**
 * @brief Function implementing the SendSPITask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_SendSPIdataTask */
void SendSPIdataTask(void *argument) {
	/* USER CODE BEGIN SendSPIdataTask */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(50); // ms to ticks

	// Infinite loop for continuous SPI communication
	xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		if (osMutexAcquire(spiMutexHandle, osWaitForever) == osOK) {
			if (memcmp(data, lastData, sizeof(data)) != 0) // Compare the arrays
					{
				sendSPIdata(data, 3); // Implement this function to send data using SPI
				memcpy(lastData, data, sizeof(data)); // Copy data to lastData
			}
			osMutexRelease(spiMutexHandle);
		}
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
	/* USER CODE END SendSPIdataTask */
}

/* USER CODE BEGIN Header_TrafficLightTask */
/**
 * @brief Function implementing the TrafficLight thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_TrafficLightTask */
void TrafficLightTask(void *argument) {
	/* USER CODE BEGIN TrafficLightTask */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10); // ms to ticks
	xLastWakeTime = xTaskGetTickCount();
	initialize(&tl);
	/* Infinite loop */
	for (;;) {
#ifdef RUN_TEST_PROGRAM
	  vTaskDelete(NULL);
	#else
		trafficLightStateMachine(&tl);
		updateInputs(&tl);
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
#endif
	}
	/* USER CODE END TrafficLightTask */
}

/* USER CODE BEGIN Header_ToggleLEDTask */
/**
 * @brief Function implementing the ToggleLED thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ToggleLEDTask */
void ToggleLEDTask(void *argument) {
	/* USER CODE BEGIN ToggleLEDTask */
	const TickType_t xFrequency = toggleFreq;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	//uint8_t ledID = *((uint8_t*) argument);
	/* Infinite loop */
	for (;;) {
		toggleLED(ledID);
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
	/* USER CODE END ToggleLEDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void wait_cycles(uint32_t n) {
	uint32_t l = n / 3; //cycles per loop is 3
	asm volatile( "0:" "SUBS %[count], 1;" "BNE 0b;" :[count]"+r"(l) );
}
/* USER CODE END Application */

