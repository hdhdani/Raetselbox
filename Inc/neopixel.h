/*
 * Neopixel.h
 *
 *  Created on: Dec 19, 2019
 *      Author: Daniel 
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include "stm32l1xx_hal.h"

 // 20 nops warten, inline funktion ging nicht wirklich ( zeit passte nicht)
#define  w820nop __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");	//20 nops
#define  w805nop __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");

#define  w807nop __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");
#define  w808nop __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");

#define  w810nop  __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");
//14 nops warten
#define  w814nop	__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");


//Farben
#define color_gruen 	0x0000FF
#define color_rot   	0x00FF00
#define color_blau 		0xFF0000
#define color_weiss		0xFFFFFF
#define color_violett	0x00FFFF
#define color_gelb   	0xFF00FF
#define color_orange 	0xFFA500







class Neopixel {
private:
	GPIO_TypeDef *myGPIOx;
	 uint16_t myGPIO_Pin;
	 uint8_t myledcounts;


 public:
	void init( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,  uint8_t ledcounts);
	void set(bool state, uint8_t num_of_leds);
	void clear();
	void setcolor(uint32_t* colors);
	uint8_t getledcount(); 
	
};

#endif /* NEOPIXEL_H_ */
