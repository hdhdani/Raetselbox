/*
 * tempsens.h
 *
 *  Created on: 14.12.2020
 *      Author: Melzer-PC
 */

#ifndef INC_TEMPSENS_H_
#define INC_TEMPSENS_H_

#include "stm32l1xx_hal.h"


class Tempsens{
private:
	uint8_t Adress;
	I2C_HandleTypeDef Hi2c;


public:

	HAL_StatusTypeDef init(I2C_HandleTypeDef hi2c, uint8_t adress);
	float readtemp(void);
	float readhumidity(void);



};


#define SI7021_MEASRH_HOLD_CMD                                                 \
  0xE5 /**< Measure Relative Humidity, Hold Master Mode */
#define SI7021_MEASRH_NOHOLD_CMD                                               \
  0xF5 /**< Measure Relative Humidity, No Hold Master Mode */
#define SI7021_MEASTEMP_HOLD_CMD                                               \
  0xE3 /**< Measure Temperature, Hold Master Mode */
#define SI7021_MEASTEMP_NOHOLD_CMD                                             \
  0xF3 /**< Measure Temperature, No Hold Master Mode */
#define SI7021_READPREVTEMP_CMD                                                \
  0xE0 /**< Read Temperature Value from Previous RH Measurement */
#define SI7021_RESET_CMD 0xFE           /**< Reset Command */
#define SI7021_WRITERHT_REG_CMD 0xE6    /**< Write RH/T User Register 1 */
#define SI7021_READRHT_REG_CMD 0xE7     /**< Read RH/T User Register 1 */
#define SI7021_WRITEHEATER_REG_CMD 0x51 /**< Write Heater Control Register */
#define SI7021_READHEATER_REG_CMD 0x11  /**< Read Heater Control Register */
#define SI7021_REG_HTRE_BIT 0x02        /**< Control Register Heater Bit */
#define SI7021_ID1_CMD 0xFA0F           /**< Read Electronic ID 1st Byte */
#define SI7021_ID2_CMD 0xFCC9           /**< Read Electronic ID 2nd Byte */
#define SI7021_FIRMVERS_CMD 0x84B8      /**< Read Firmware Revision */

#endif /* INC_TEMPSENS_H_ */
