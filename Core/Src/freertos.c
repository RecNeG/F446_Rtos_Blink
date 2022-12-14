/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "string.h"
#include "userLed.h"
#include "userUart.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

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
extern UART_HandleTypeDef huart2;
/* USER CODE END Variables */
/* Definitions for LedTask */
osThreadId_t LedTaskHandle;
const osThreadAttr_t LedTask_attributes = {
  .name = "LedTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for EchoTask */
osThreadId_t EchoTaskHandle;
const osThreadAttr_t EchoTask_attributes = {
  .name = "EchoTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartLedTask(void *argument);
void StartEchoTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of LedTask */
  LedTaskHandle = osThreadNew(StartLedTask, NULL, &LedTask_attributes);

  /* creation of EchoTask */
  EchoTaskHandle = osThreadNew(StartEchoTask, NULL, &EchoTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartLedTask */
/**
  * @brief  Function implementing the LedTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLedTask */
void StartLedTask(void *argument)
{
  /* USER CODE BEGIN StartLedTask */
	
	userLed.CurrentTick=0;
	userLed.GPIO_Pin=GPIO_PIN_5; //LD2_Pin
	userLed.GPIO_Port=GPIOA; //LD2_GPIO_Port
	userLed.HighTick=100;	//OsTick (ms)
	userLed.LowTick=100;
	userLed.Preload=LPL_Disable;
	userLed.GPIO_State=LS_Low;
	
	uint32_t _sleepTime;
  /* Infinite loop */
  for(;;)
  {
		if(userUart.EchoState==UES_Enable)
			_sleepTime=LedHandler(&userLed);
		else
			_sleepTime=LedHandlerStatic(&userLed);
		
		uint32_t _threadFlag = osThreadFlagsWait(OSTF_KILLECHO|OSTF_STARTECHO,osFlagsWaitAny,_sleepTime);
		switch (_threadFlag)
		{
			case OSTF_KILLECHO:
			{
				osThreadTerminate(EchoTaskHandle);
				userUart.EchoState=UES_Disable;
				break;
			}
			case OSTF_STARTECHO:
			{
				EchoTaskHandle = osThreadNew(StartEchoTask, NULL, &EchoTask_attributes);
				userUart.EchoState=UES_Enable;
				break;
			}
		}
  }
  /* USER CODE END StartLedTask */
}

/* USER CODE BEGIN Header_StartEchoTask */
/**
* @brief Function implementing the EchoTask thread.
* @param argument: Not used
* @retval None
*/

/* USER CODE END Header_StartEchoTask */
void StartEchoTask(void *argument)
{
  /* USER CODE BEGIN StartEchoTask */
	uint32_t _sleepTime=0;
  /* Infinite loop */
  for(;;)
  {
		uint32_t _threadFlag = osThreadFlagsWait(OSTF_CHECKREQUEST,osFlagsWaitAny,_sleepTime);
		switch (_threadFlag)
		{
			case OSTF_CHECKREQUEST:
			{
				_sleepTime=5;
				break;
			}
			default:
			{
				_sleepTime=osWaitForever;
				EchoHandler(&userUart,&userLed);
				break;
			}
		}		
  }
  /* USER CODE END StartEchoTask */
}

/* Private application code --------------------------------------------------*/

/* USER CODE END Application */

