#include "gpio.h"
#include "PIRMotionSensor.h"

void PIRMotionSensor_Init()
{
  uint8_t pirState = GPIO_PIN_RESET;
  uint8_t val = GPIO_PIN_RESET;
  
	while(1)
	{	
		val = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);	
		if(val == GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_SET);
			if(pirState == GPIO_PIN_RESET)
			{
				pirState = GPIO_PIN_SET;
			}
		}
		else
		{
			HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_RESET);
			if(pirState == GPIO_PIN_SET)
			{
				pirState = GPIO_PIN_RESET;
			}
		}			
	}	
}

