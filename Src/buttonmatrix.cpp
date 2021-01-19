/*
 * buttonmatrix.cpp
 *
 *  Created on: Dec 15, 2020
 *      Author: Melzer-PC
 */

#include "buttonmatrix.h"
#include "stm32l1xx_hal.h"
#include "gpio.h"


// nur ein Taster gleichzeitig drücken 
//Andernfalls wird ( wenn in einer Reihe) der wert falsch ausgelesen
//oder bei unterschiedlichen reihen nur der erste wert.
/**
 * @brief auslesen, ob und welcher Taster gedrückt
 * 
 * @return uint8_t Nummer des gedrückter Taster
 */
uint8_t getButton()
{
	uint8_t button=0;


	for(uint8_t i=0; i<4; i++) //0...3
	{
		HAL_GPIO_WritePin( SW_1_GPIO_Port ,SW_1_Pin, (GPIO_PinState)(0b0001& (1<<(i))));
		HAL_GPIO_WritePin( SW_2_GPIO_Port ,SW_2_Pin, (GPIO_PinState)(0b0010& (1<<(i))));
		HAL_GPIO_WritePin( SW_3_GPIO_Port ,SW_3_Pin, (GPIO_PinState)(0b0100& (1<<(i))));
		HAL_GPIO_WritePin( SW_4_GPIO_Port ,SW_4_Pin, (GPIO_PinState)(0b1000& (1<<(i))));


		button = 0;
		button|= HAL_GPIO_ReadPin(SW_5_GPIO_Port,SW_5_Pin);
		button|= HAL_GPIO_ReadPin(SW6_GPIO_Port,SW6_Pin)<<1;
		button|= HAL_GPIO_ReadPin(SW_7_GPIO_Port,SW_7_Pin)<<2;
		button|= HAL_GPIO_ReadPin(SW_8_GPIO_Port,SW_8_Pin)<<3;

		if (button >0 )
		{
			//		reihe    1     		2			3*  0b100>>2 -> 0b1			0b1000>>1 = 4
			button= (i*4)+(button& 1) + (button& 2) + 3*((button& 4)>>2) +((button& 8)>>1);
			break;

		}




	}



//0 = kein Taster gedrückt, 1-16 -> gedrückte Taster wie auf PCB steht  
	return button; 
}









