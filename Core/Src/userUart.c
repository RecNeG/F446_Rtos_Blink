#include "userUart.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "userLed.h"
#include "cmsis_os.h"

userUart_t userUart;

extern osThreadId_t EchoTaskHandle;
extern osThreadId_t LedTaskHandle;

void UartHandler(userUart_t *_uart)
{
	switch ((uint8_t)(_uart->EchoState))
	{
		case UES_Enable:
		{
			
			if(strncmp((char*)(_uart->RxBuf),"stop",_uart->RxBufCounter)!=0)
			{
				osThreadFlagsSet(EchoTaskHandle,OSTF_CHECKREQUEST);
				return;
			}
			else if(_uart->RxBufCounter==4)
			{
				osThreadFlagsSet(LedTaskHandle,OSTF_KILLECHO);
				memset(_uart->RxBuf,0,_uart->RxBufCounter);
				_uart->RxBufCounter=0;
			}
			break;
		}
		case UES_Disable:
		{
			if(strncmp((char*)(_uart->RxBuf),"start",_uart->RxBufCounter)!=0)
			{
				memset(_uart->RxBuf,0,_uart->RxBufCounter);
				_uart->RxBufCounter=0;
			}
			else if(_uart->RxBufCounter==5)
			{
				osThreadFlagsSet(LedTaskHandle,OSTF_STARTECHO);
				memset(_uart->RxBuf,0,_uart->RxBufCounter);
				_uart->RxBufCounter=0;
				return;
			}
			break;
		}
	}
}

void EchoHandler(userUart_t *_uart,void *_led)
{
	if(strncmp((char*)(_uart->RxBuf),"led",3)==0)
	{
		if(strncmp((char*)(&_uart->RxBuf[3]),"on=",3)==0)
		{
			uint32_t _tick=atoi((char*)&_uart->RxBuf[6]);
			if(_tick!=0)
			{
				LedSetHighTick((userLed_t*)_led,_tick);
			}
		}
		else if (strncmp((char*)(&_uart->RxBuf[3]),"off=",4)==0)
		{
			uint32_t _tick=atoi((char*)&_uart->RxBuf[7]);
			if(_tick!=0)
			{
				LedSetLowTick((userLed_t*)_led,_tick);
			}
		}
	}
	//echo
	memcpy(_uart->TxBuf,_uart->RxBuf,_uart->RxBufCounter);
	_uart->TxBufCounter = _uart->RxBufCounter;
	HAL_UART_Transmit_IT(&huart2,_uart->TxBuf,_uart->TxBufCounter);
	//clear
	memset(_uart->RxBuf,0,_uart->RxBufCounter);
	_uart->RxBufCounter=0;
	HAL_UART_AbortReceive_IT(&huart2);
	HAL_UART_Receive_IT(&huart2,&userUart.RxBuf[userUart.RxBufCounter],1);
}
/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	userUart.RxBufCounter++;
	UartHandler(&userUart);
  HAL_UART_Receive_IT(&huart2,&userUart.RxBuf[userUart.RxBufCounter],1);
	
}