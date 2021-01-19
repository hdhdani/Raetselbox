/*
 * gesturesens.cpp
 *
 *  Created on: 14.12.2020
 *      Author: Melzer-PC
 */
#include "gesturesens.h"

	HAL_StatusTypeDef gesturesens::init(I2C_HandleTypeDef hi2c, uint8_t adress)
	{
		this->Hi2c = hi2c;
		this->Adress = adress <<1; // Adresse, um 1 nach links geshiftet, da 7Bit Adresse
		//volatile HAL_StatusTypeDef i2cstate;

		HAL_StatusTypeDef I2cready =HAL_I2C_IsDeviceReady(&this->Hi2c, this->Adress,2,100);
		if(I2cready != HAL_OK)
		{
			return I2cready; // Display not found
		}

		return HAL_OK;
	}


	HAL_StatusTypeDef gesturesens::writecmd(uint8_t command)
	{
		uint8_t sendreg[2];
		sendreg[0]= 0;
		sendreg[1]= command;
		return HAL_I2C_Master_Transmit(&this->Hi2c, this->Adress, sendreg, 2, 10);

	}
