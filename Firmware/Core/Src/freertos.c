/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "Libraries/SD.h"
#include "Libraries/BMP280.h"
#include "Libraries/SBUS.h"
#include "Libraries/LED.h"
#include "Libraries/BNO050.h"
#include "Libraries/PWM.h"
#include "Libraries/Lora_LR03.h"
#include "libNMEA.h"
/* USER CODE END Includes */
#include "Flight_Management_Control/flight_management.h"


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId Task_1msHandle;
osThreadId Task_10msHandle;
osThreadId Task_100msHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void fTask_1ms(void const * argument);
void fTask_10ms(void const * argument);
void fTask_100ms(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* definition and creation of Task_1ms */
  osThreadDef(Task_1ms, fTask_1ms, osPriorityNormal, 0, 128);
  Task_1msHandle = osThreadCreate(osThread(Task_1ms), NULL);

  /* definition and creation of Task_10ms */
  osThreadDef(Task_10ms, fTask_10ms, osPriorityNormal, 0, 128);
  Task_10msHandle = osThreadCreate(osThread(Task_10ms), NULL);

  /* definition and creation of Task_100ms */
  osThreadDef(Task_100ms, fTask_100ms, osPriorityNormal, 0, 128);
  Task_100msHandle = osThreadCreate(osThread(Task_100ms), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_fTask_1ms */
/**
* @brief Function implementing the Task_1ms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fTask_1ms */
void fTask_1ms(void const * argument)
{
  /* USER CODE BEGIN fTask_1ms */
  /* Infinite loop */

  for(;;)
  {
	TimeOn_Counter++;
	LED_Tasks();
	SBUS_IntegrityVerification();
	NMEA_process_task();
    osDelay(1);
  }
  /* USER CODE END fTask_1ms */
}

/* USER CODE BEGIN Header_fTask_10ms */
/**
* @brief Function implementing the Task_10ms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fTask_10ms */
void fTask_10ms(void const * argument)
{
  /* USER CODE BEGIN fTask_10ms */
	FlightControl_inits();
  /* Infinite loop */
  for(;;)
  {
	BNO_Tasks();
	BMP280_calculate();

	FlightTaskAttitude();

	PWM_Assign();
	SD_blackbox_write();
    osDelay(10);
  }
  /* USER CODE END fTask_10ms */
}

/* USER CODE BEGIN Header_fTask_100ms */
/**
* @brief Function implementing the Task_100ms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fTask_100ms */
void fTask_100ms(void const * argument)
{
  /* USER CODE BEGIN fTask_100ms */
  /* Infinite loop */
  for(;;)
  {
	LR03_StateMachine();
    osDelay(100);
  }
  /* USER CODE END fTask_100ms */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
