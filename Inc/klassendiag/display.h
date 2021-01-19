/*
 * display.h
 *
 *  Created on: 13.12.2020
 *      Author: Melzer-PC
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stm32l1xx_hal.h"
#include <string.h>
#include"sincoslookup.h"
#include "gfxfont.h"


class Display{
private:
	uint8_t Adress;
	I2C_HandleTypeDef Hi2c;
	uint8_t Displaybuffer[128*64/8]; // buffer in dem alle neuen Pixel gespeichert werden
	GFXfont font;



public:
	HAL_StatusTypeDef writecmd(uint8_t command);
	HAL_StatusTypeDef init(I2C_HandleTypeDef hi2c, uint8_t adress);

	void writedata(uint8_t reg, uint8_t* data, uint16_t count);
	void updatescreen(void);
	void drawpixel(uint8_t x, uint8_t y);
	void drawHline(uint8_t x, uint8_t y, uint8_t length);
	void drawVline(uint8_t x, uint8_t y, uint8_t length);
	void drawrectangle(uint8_t x, uint8_t y, uint8_t length, uint8_t height);
	void line(int x0, int y0, int x1, int y1);
	uint8_t writestring(uint8_t x, uint8_t y, const char* string);
	void setfont(uint8_t font); //GFXfont font);
	uint8_t writechar(uint8_t x, uint8_t y, char c);
	uint8_t writechartiny(uint8_t x, uint8_t y, char c);
	void cleanbuffer(void);

};


#endif /* INC_DISPLAY_H_ */
