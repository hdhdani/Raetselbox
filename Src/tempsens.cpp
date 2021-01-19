/*
 * tempsens.cpp
 *
 *  Created on: 14.12.2020
 *      Author: Melzer-PC
 */

#include"tempsens.h"



/*@brief Sensor Init
 * @note	Check ob Sensor angeschlossen & Adresse sowie i2c handler  zuweisen
 * @param
 * @retval HAL_ok/ HAL_error
 * */
	HAL_StatusTypeDef Tempsens::init(I2C_HandleTypeDef hi2c, uint8_t adress)
	{
		this->Hi2c = hi2c;
		this->Adress = adress <<1; // Adresse, um 1 nach links geshiftet, da 7Bit Adresse

		HAL_StatusTypeDef I2cready =HAL_I2C_IsDeviceReady(&this->Hi2c, this->Adress,2,100);

		if(I2cready != HAL_OK)
		{
			return I2cready; // Tempsensor not found
		}

		return HAL_OK;
	}


	/*@brief	lese Temperatur als blocking function
	 * @note
	 * @param none
	 * @retval temperatur  -10...85°C
	 * */
	float Tempsens::readtemp(void)
	{
		// volatile HAL_StatusTypeDef i2crec;
		uint8_t pdata= SI7021_MEASTEMP_NOHOLD_CMD;
		uint8_t data[2];
		HAL_I2C_Master_Transmit(&this->Hi2c,this->Adress, &pdata,1,100);

		HAL_Delay(20);
		/*i2crec= */ HAL_I2C_Master_Receive(&this->Hi2c,this->Adress | 0x01 , data, 2, 100);

		int64_t temp_code = (data[0] << 8 | data[1]);
		float temp = ((175.72 * (float)temp_code) / 65536.0) - 46.85;



		return temp;
	}

	/*@brief	lese Luftfeuchtigkeit als blocking function
		 * @note
		 * @param none
		 * @retval relative Luftfeuchtigkeit  0...80%
		 * */
	float Tempsens::readhumidity(void)
	{
		uint8_t pdata= SI7021_MEASTEMP_HOLD_CMD;
		uint8_t data[2];
		HAL_I2C_Master_Transmit(&this->Hi2c,this->Adress, &pdata,1,100);

		HAL_Delay(20);
		HAL_I2C_Master_Receive(&this->Hi2c,this->Adress | 0x01 , data, 2, 100);

		int64_t humid_code = (data[0] << 8 | data[1]);
		float humid = ((125.0 * (float)humid_code) / 65536.0) - 6.0;



		return humid;;
	}
