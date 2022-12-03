#include "userUart.h"
#include "usart.h"
#include "string.h"
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