#include "i2c.h"
#include "gpio.h"
#include "TSL2561.h"

#define TSL2561_DELAY_INTTIME_13MS    (15)
#define TSL2561_DELAY_INTTIME_101MS   (120)
#define TSL2561_DELAY_INTTIME_402MS   (450)

void TSL2561_Init()
{
	
}


void TSL2561_EnableSensor()
{
	static uint8_t TxBuffer[1], RxBuffer[1]; 
	TxBuffer[0] = TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL, TSL2561_CONTROL_POWERON;
	
	//HAL_I2C_Master_Transmit(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), TxBuffer, 1, 10000);
	HAL_Delay(25);	
	
	//HAL_I2C_Master_Receive(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), RxBuffer, 1, 10000);

}

void TSL2561_DisableSensor()
{
	
}