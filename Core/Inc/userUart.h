#ifndef USERUART_H
#define USERUART_H

#include "stm32f4xx_hal.h"
#include "stdint.h"

#define OSTF_CHECKREQUEST 	1

enum usartEcho_t : uint8_t
{
	UES_Enable  =0,
  UES_Disable =1
};


typedef struct 
{
	enum usartEcho_t EchoState;
	uint8_t RxBuf[64];
	uint8_t RxBufCounter;
	uint8_t TxBuf[64];
	uint8_t TxBufCounter;
}userUart_t;

extern userUart_t userUart;
extern void UartHandler(userUart_t *_uart);
extern void EchoHandler(userUart_t *_uart,void *_led);

#endif