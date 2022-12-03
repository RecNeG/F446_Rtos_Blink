#ifndef USERLED_H
#define USERLED_H

#include "stm32f4xx_hal.h"
#include "stdint.h"


#define LPLTICK	1
enum ledPreLoad_t : uint8_t
{
  LPL_Disable =1,
  LPL_Enable 	=2
};

enum ledState_t : uint8_t
{
  LS_Low =1,
  LS_High =2
};

typedef struct 
{
	GPIO_TypeDef* 		GPIO_Port; 
	uint16_t 					GPIO_Pin;
	enum ledState_t 	State;		//Led durumu, suandaki durumu Kapali/Acik
	uint32_t 					LowTick;	//Nekadar kapali kalacagi
	uint32_t 					HighTick; //Nekadar acik kalacagi
	enum ledPreLoad_t Preload;	//Yeni ayarlanan degerin onceki deger uygulanmadan isleme alinip alinmayacagini belirtir.
	uint32_t					CurrentTick;	//Ledin suanki durumunda ne kadar sure gecirdigini saklar
}userLed_t;


extern uint32_t LedHandler(userLed_t *_led);
extern void LedSetHighTick(userLed_t *_led,uint32_t _highTick);
extern void LedSetLowTick(userLed_t *_led,uint32_t _lowTick);

#endif