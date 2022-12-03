#include "userLed.h"

userLed_t userLed;
/**
  * @brief  Led time handler functiopn.
  * @param  userLed_t  Pointer
  * @retval TickCount of delay;
  */
uint32_t LedHandler(userLed_t *_led)
{
	switch ((uint8_t)(_led->Preload))
	{
		case LPL_Enable:
		{
			_led->CurrentTick+=LPLTICK;
			if(_led->GPIO_State==LS_High)
			{
				if(_led->CurrentTick>=_led->HighTick)
				{
					_led->CurrentTick=0;
					_led->GPIO_State=LS_Low;
					HAL_GPIO_WritePin(_led->GPIO_Port,_led->GPIO_Pin,GPIO_PIN_RESET);
				}
			}
			else
			{
				if(_led->CurrentTick>=_led->LowTick)
				{
					_led->CurrentTick=0;
					_led->GPIO_State=LS_High;
					HAL_GPIO_WritePin(_led->GPIO_Port,_led->GPIO_Pin,GPIO_PIN_SET);
				}
			}
			return LPLTICK;
		}
		case LPL_Disable:
		{
			if(_led->GPIO_State==LS_High)
			{
				_led->GPIO_State=LS_Low;
				HAL_GPIO_WritePin(_led->GPIO_Port,_led->GPIO_Pin,GPIO_PIN_RESET);
				return _led->LowTick;
			}
			else
			{
				_led->GPIO_State=LS_High;
				HAL_GPIO_WritePin(_led->GPIO_Port,_led->GPIO_Pin,GPIO_PIN_SET);
				return _led->HighTick;
			}
		}
	}
	return 0;
}

uint32_t LedHandlerStatic(userLed_t *_led)
{

	if(_led->GPIO_State==LS_High)
	{
		_led->GPIO_State=LS_Low;
		HAL_GPIO_WritePin(_led->GPIO_Port,_led->GPIO_Pin,GPIO_PIN_RESET);
	}
	else
	{
		_led->GPIO_State=LS_High;
		HAL_GPIO_WritePin(_led->GPIO_Port,_led->GPIO_Pin,GPIO_PIN_SET);
	}
	return 1000;
}

void LedSetLowTick(userLed_t *_led,uint32_t _lowTick)
{
	_led->LowTick=_lowTick;
}

void LedSetHighTick(userLed_t *_led,uint32_t _highTick)
{
	_led->HighTick=_highTick;
}
