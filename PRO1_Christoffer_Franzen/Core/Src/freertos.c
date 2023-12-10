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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticMutexDef_t;
/* USER CODE BEGIN PTD */

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

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SendSPITask */
osThreadId_t SendSPITaskHandle;
const osThreadAttr_t SendSPITask_attributes = {
  .name = "SendSPITask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for RoadTask1 */
osThreadId_t RoadTask1Handle;
const osThreadAttr_t RoadTask1_attributes = {
  .name = "RoadTask1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for RoadTask2 */
osThreadId_t RoadTask2Handle;
const osThreadAttr_t RoadTask2_attributes = {
  .name = "RoadTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for spiMutex */
osMutexId_t spiMutexHandle;
osStaticMutexDef_t spiMutexControlBlock;
const osMutexAttr_t spiMutex_attributes = {
  .name = "spiMutex",
  .cb_mem = &spiMutexControlBlock,
  .cb_size = sizeof(spiMutexControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void wait_cycles(uint32_t n);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void SendSPIdataTask(void *argument);
void RoadCrossingTask1(void *argument);
void RoadCrossingTask2(void *argument);

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
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of SendSPITask */
  SendSPITaskHandle = osThreadNew(SendSPIdataTask, NULL, &SendSPITask_attributes);

  /* creation of RoadTask1 */
  RoadTask1Handle = osThreadNew(RoadCrossingTask1, NULL, &RoadTask1_attributes);

  /* creation of RoadTask2 */
  RoadTask2Handle = osThreadNew(RoadCrossingTask2, NULL, &RoadTask2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
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
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10); // ms to ticks

	/* Infinite loop */
	xLastWakeTime = xTaskGetTickCount();

	for (;;) {
#ifdef RUN_TEST_PROGRAM
	  Test_program();
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
void SendSPIdataTask(void *argument)
{
  /* USER CODE BEGIN SendSPIdataTask */
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(50); // ms to ticks

    // Infinite loop for continuous SPI communication
    xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        if (osMutexAcquire(spiMutexHandle, osWaitForever) == osOK) {
            if (memcmp(data, lastData, sizeof(data)) != 0) // Compare the arrays
            {
                sendSPIdata(data, 3);                 // Implement this function to send data using SPI
                memcpy(lastData, data, sizeof(data)); // Copy data to lastData
            }
            osMutexRelease(spiMutexHandle);
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
  /* USER CODE END SendSPIdataTask */
}

/* USER CODE BEGIN Header_RoadCrossingTask1 */
/**
 * @brief Function implementing the RoadTask1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RoadCrossingTask1 */
void RoadCrossingTask1(void *argument)
{
  /* USER CODE BEGIN RoadCrossingTask1 */
	/* Infinite loop */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10); // ms to ticks
	xLastWakeTime = xTaskGetTickCount();
	for (;;) {
#ifdef RUN_TEST_PROGRAM
	  vTaskDelete(NULL);
	#else
		TrafficLightStateMachine();
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
#endif
	}
  /* USER CODE END RoadCrossingTask1 */
}

/* USER CODE BEGIN Header_RoadCrossingTask2 */
/**
 * @brief Function implementing the RoadTask2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RoadCrossingTask2 */
void RoadCrossingTask2(void *argument)
{
  /* USER CODE BEGIN RoadCrossingTask2 */
	/* Infinite loop */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10); // ms to ticks
	xLastWakeTime = xTaskGetTickCount();
	for (;;) {
#ifdef RUN_TEST_PROGRAM
		  vTaskDelete(NULL);
		#else
		TrafficLightStateMachine2();
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
#endif
	}
  /* USER CODE END RoadCrossingTask2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void wait_cycles(uint32_t n) {
	uint32_t l = n / 3; //cycles per loop is 3
	asm volatile( "0:" "SUBS %[count], 1;" "BNE 0b;" :[count]"+r"(l) );
}
/* USER CODE END Application */

