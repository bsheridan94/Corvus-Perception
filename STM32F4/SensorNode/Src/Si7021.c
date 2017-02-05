#include "i2c.h"
#include "gpio.h"
#include "Si7021.h"
#include "usart.h"

static float cacheTemp = 0.0;

bool Si7021_Init()
{
	uint8_t TxBuffer[2], RxBuffer[2]; 
	
	//Send Reset
	TxBuffer[0] = SI7021_RESET_CMD;
	HAL_I2C_Master_Transmit(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), TxBuffer, 1, 10000);
	HAL_Delay(50);		 
	
	// Read back register
	HAL_I2C_Mem_Read(&hi2c1,  (SI7021_DEFAULT_ADDRESS << 1), SI7021_READRHT_REG_CMD, I2C_MEMADD_SIZE_8BIT, RxBuffer, 1, 10000);
	
	//Check register status
	
	if(RxBuffer[0] != 0x3A)
	{
		return false;
	}
	else
	{
		return true;
	}
}


uint16_t Si7021_ReadTemp(void)
{
	static uint8_t TxBuffer[1], RxBuffer[3]; 
	TxBuffer[0] = SI7021_MEASTEMP_NOHOLD_CMD;
	
	HAL_I2C_Master_Transmit(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), TxBuffer, 1, 10000);
	HAL_Delay(25);	
	
	HAL_I2C_Master_Receive(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), RxBuffer, 3, 10000);
	//HAL_I2C_Mem_Read(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), SI7021_MEASTEMP_NOHOLD_CMD, I2C_MEMADD_SIZE_8BIT, RxBuffer, 3, 10000);
	
	uint16_t temp = (RxBuffer[0] << 8) | RxBuffer[1];
	uint8_t checkSum = RxBuffer[2];
	
	return temp;
}



uint16_t Si7021_ReadHumidity(void)
{
	static uint8_t TxBuffer[1], RxBuffer[3]; 
	
	//Send Reset
	TxBuffer[0] = SI7021_MEASRH_NOHOLD_CMD;
	
	HAL_I2C_Master_Transmit(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), TxBuffer, 1, 10000);
	HAL_Delay(25);	
	
	HAL_I2C_Master_Receive(&hi2c1, (SI7021_DEFAULT_ADDRESS << 1), RxBuffer, 3, 10000);
	
	static uint16_t humidity =0;
	humidity = (RxBuffer[0] << 8) | RxBuffer[1];
	uint8_t checkSum = RxBuffer[2];
	
	return humidity;
}


void Si7021_ReadSensor(void)
{
	float temp = 0.0;
	float humidity = 0.0;
	
	//Remove conversions, floating point just for debug
	temp = Si7021_ReadTemp();
	temp *= 175.72;
	temp /= 65536;
	temp -= 46.85;
	
	
	humidity = Si7021_ReadHumidity();
	humidity *= 125;
	humidity /= 65536;
	humidity -= 6;
	
	humidity = 0.0;
	
}
