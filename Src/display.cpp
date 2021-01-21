/*
 * Display.cpp
 *
 *  Created on: 13.12.2020
 *      Author: Melzer-PC
 *
 *      Display: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */

#include "Display.h"
#include <math.h>

#include "fonts.h"
#include "tinyfont.h"


    /**
     * @brief schreibe Kommando über I2C raus
     *
     * @param command
     * @return HAL_StatusTypeDef
     */
	HAL_StatusTypeDef Display::writecmd(uint8_t command)
	{
		uint8_t sendreg[2];
		sendreg[0]= 0;
		sendreg[1]= command;
		return HAL_I2C_Master_Transmit(&this->Hi2c, this->Adress, sendreg, 2, 10);

	}



    /**
     * @brief initialisiere Dispaly
     *
     * @param hi2c
     * @param adress
     * @return HAL_StatusTypeDef
     */
	HAL_StatusTypeDef Display::init(I2C_HandleTypeDef hi2c, uint8_t adress)
	{
		this->Hi2c = hi2c;
		this->Adress = adress <<1; // Adresse, um 1 nach links geshiftet, da 7Bit Adresse

		HAL_StatusTypeDef I2cready =HAL_I2C_IsDeviceReady(&this->Hi2c, this->Adress,2,100);
		this-> font= Picopixel;
		if(I2cready != HAL_OK)
		{
			return I2cready; // Display not found
		}


		//Display found, Init OLED
		//setze diverse konfigurationen
		writecmd(0xAE); //Display off

		writecmd(0x20); //Set Memory Addressing Mode
		writecmd(0x00); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid

		writecmd(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

		writecmd(0xC8); //Set COM Output Scan Direction

		writecmd(0x00); //---set low column address
		writecmd(0x10); //---set high column address

		writecmd(0x40); //--set start line address
		writecmd(0x81); //--set contrast control register
		writecmd(0xFF);

		writecmd(0xA1); //--set segment re-map 0 to 127
		writecmd(0xA6); //--set normal Display
		writecmd(0xA8); //--set multiplex ratio(1 to 64)
		writecmd(0x3F); //
		writecmd(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

		writecmd(0xD3); //-set Display offset
		writecmd(0x00); //-not offset

		writecmd(0xD5); //--set Display clock divide ratio/oscillator frequency
		writecmd(0xF0); //--set divide ratio
		writecmd(0xD9); //--set pre-charge period
		writecmd(0x22); //
		writecmd(0xDA); //--set com pins hardware configuration
		writecmd(0x12);
		writecmd(0xDB); //--set vcomh
		writecmd(0x20); //0x20,0.77xVcc
		writecmd(0x8D); //--set DC-DC enable
		writecmd(0x14); //
		writecmd(0xAF); //--turn on SSD1306 panel
		writecmd(0x2E); // deactivate scroll

		cleanbuffer();
		updatescreen();



	return HAL_OK;
	}

    /**
     * @brief Schreibe Buffer zu Display
     *
     */
	void Display::updatescreen(void)
	{
		writecmd(0x40); //--set start line address 
		HAL_I2C_Mem_Write(&this->Hi2c, this->Adress,0x40, 1, this->Displaybuffer, 128*8,100);
	}


    /**
     * @brief Schreibe Daten an Display
     *
     * @param reg
     * @param data
     * @param count
     */
	void Display::writedata(uint8_t reg, uint8_t* data, uint16_t count)
	{
		uint8_t sendreg[256];
		sendreg[0] = reg;
		uint8_t i;

		for(i = 0; i < count; i++)  sendreg[i+1] = data[i];
		HAL_I2C_Master_Transmit(&this->Hi2c, this->Adress, sendreg, count+1, 10);
	}


    /**
     * @brief Zeichne einen Pixel in Buffer
     *
     * @param x Koordinate
     * @param y Koordinate
     */
	void Display::drawpixel(uint8_t x, uint8_t y)
	{
		//dispsize 128 * 64
		if (x >127 || y>63){
			return;
		}
		uint16_t byte= x + (y/8)*128; // anpassen von Koordinaten an Displayposition, für Horizontal-mode
		this->Displaybuffer[byte] |= 1<<(y%8);
	}


    /**
     * @brief Zeichen horizontale Linie in Buffer
     *
     * @param x Startkoordinate
     * @param y Koordinate
     * @param length Länge
     */
	void Display::drawHline(uint8_t x, uint8_t y, uint8_t length)
	{
		if((x+length)> 128 || y>64)
		{
			return;
		}

		for(uint8_t i=0; i<length; i++)
		{
			this->drawpixel(x+i,y);
		}
	}


    /**
     * @brief Zeichne vertikale Linie in Buffer
     *
     * @param x	Koordinate
     * @param y	Startkoordinate
     * @param length	 Länge
     */
	void Display::drawVline(uint8_t x, uint8_t y, uint8_t length)
	{
		if((y+length)> 64 || x>128)
		{
			return;
		}

		for(uint8_t i=0; i<length; i++)
		{
			this->drawpixel(x,y+i);
		}
	}


    /**
     * @brief Zeichne ein Rechteck in Buffer
     *
     * @param x		Startkoordinate ( links)
     * @param y		Starkoordinate ( oben)
     * @param length  länge
     * @param height  höhe
     */
	void Display::drawrectangle(uint8_t x, uint8_t y, uint8_t length, uint8_t height)
	{
		drawHline(x,y,length);
		drawHline(x,y+height,length);
		drawVline(x,y,height);
		drawVline(x+length,y,height);
	}


    /**
     * @brief Zeichne Linie in Buffer
     *
     * @param x0 	x-Startkoordinate
     * @param y0	y-Startkoordinate
     * @param x1	x-Endkoordinate
     * @param y1	y-Endkoordinate
     */
	void Display::line(int x0, int y0, int x1, int y1) //bresenham
	{
	    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
	    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
	    int err = dx+dy, e2; /* error value e_xy */

	    while (1) {
	        drawpixel(x0,y0);
	        if (x0==x1 && y0==y1) break;
	        e2 = 2* err;
	        if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
	        if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	    }
	}


    /**
     * @brief Schreibe einen String in Buffer
     *
     * @param x	obere  X-Koordinate
     * @param y	linke  Y-koordinate
     * @param string   Zeichenkette
     * @return uint8_t neue X-koordinate
     */
	uint8_t Display::writestring(uint8_t x, uint8_t y, const char* string)
	{
		uint8_t i=0;
		uint8_t xp=x;
		uint8_t yp= y;
		while(string[i] != '\0')
		{
			xp += writechar( xp ,yp, string[i]);
			if (xp > 127 )
			{
				yp += font.yAdvance;
				xp = x;
			}
			i++;
		}
		return xp;
	}


    /**
     * @brief Schreibe ein Zeichen in Buffer
     *
     * @param x	obere X-Koordinate
     * @param y	linke Y-koordinate
     * @param c	char
     * @return uint8_t	neue X-koordinate
     */
	uint8_t  Display::writechar(uint8_t x, uint8_t y, char c)
	{

		if (c> font.last || c<font.first )
		{
			return 0 ;  // Zeichen nicht in fonts enthalten
		}
		uint8_t glyphindex = c- font.first;  // index

		GFXglyph g = font.glyph[glyphindex];

		uint8_t xpos = x+ g.xOffset;
		uint8_t ypos = y+ g.yOffset;
		//uint8_t size = g.width * g.height;
		uint8_t* gmap = font.bitmap+g.bitmapOffset;

		for(uint8_t ix=0; ix<g.width; ix++)
		{
			for(uint8_t iy=0; iy<g.height; iy++)
			{
				uint8_t mybyte= (ix +iy*g.width)/8;
				uint8_t mybit= ((ix +iy*g.width)%8);
				uint8_t pixel= gmap[mybyte] << mybit ;
				if(pixel & 0x80)
				{
					drawpixel(xpos+ix, ypos+iy);
				}
			}
		}
		return g.xAdvance; // X-offset
	}


    /**
     * @brief Fontgröße ändern
     *
     * @param size
     */
	void Display::setfont(uint8_t size)
	{  //(GFXfont font){
		if(size==1)
			this->font = Picopixel;
		else
		{
			this->font = FreeMono9pt7b;
		}
	}

    /**
     * @brief lösche Inhalt aus Buffer
     *
     */
	void Display::cleanbuffer(void)
	{
		memset(this->Displaybuffer, 0x00, sizeof(this->Displaybuffer));	// Buffer fuellen mit schwarz
	}


