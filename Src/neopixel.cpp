#include "neopixel.h"
#pragma GCC push_options
#pragma GCC optimize ("Ofast")

/*
 * Funktion der WS2812:
 * für jede Farbe 8 Bit senden -> 24Bit pro LED
 * ersten 24 gesendeten Bit für erste LED, 25-48. Bit für zweite LED usw..
 * nach den gesendeten Bits min. 50uS warten bevor neue Farbe gesendet wird.
 * Für eine "0" muss Pin  400ns High, anschließend 850nS low
 * für eine "1" muss min 850ns high, 400ns low.
 *
 *
 * */


/**
  * @brief  Initialisiere Objekt, (GPIO Pin muss bereits initialisiert sein)
  * @param  GPIOx: GPIO Port
  * @param GPIO_Pin: GPIO Pin
  * @param ledcounts: Anzahl der angeschlossenen LEDs
  *
  * @retval None
  */
void Neopixel::init( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,  uint8_t ledcounts)
{
	this->myGPIOx = GPIOx;
	this->myGPIO_Pin = GPIO_Pin;
	this->myledcounts= ledcounts;
	this->clear();
}



/**
  * @brief  Setze angegebene Anzahl an LEDs auf weiss, Reihenfolge: ab "DI" Pin fortlaufend die Anzahl an LEDs
  * @param  state: LEDs an oder aus
  * @param num_of_leds: Anzahl an LEDs die geschalten werden sollen
  *
  * @retval None
  */
void Neopixel::set(bool state, uint8_t num_of_leds)
{


	for(int i = 0; i < num_of_leds; i++) //für jede LED einmal durch
	{
		for(int j = 0; j < 24; j++) //für alle Farbbits einmal durch
		{	// 7 nops= 800ns
			myGPIOx->BSRR = myGPIO_Pin;	// Pin High
			//myGPIOx->BSRR = (uint32_t)myGPIO_Pin << 16u;	// gpio auf 0
			w807nop //w814nop	//warten
			if(state){ w810nop }
			myGPIOx->BSRR = (uint32_t)myGPIO_Pin << 16u;	// gpio auf 0
			w807nop //w814nop //warten
			if(!state){w810nop	} // wenn muss länger 0 bleiben
		}
	}
	myGPIOx->BSRR = (uint32_t)myGPIO_Pin << 16u;
	for(int k = 0; k < 199; k++){__asm__ __volatile__ ("nop");}  // am ende min 50us Warten


}



/**
  * @brief Schalte alle LEDs aus
  * @param  None
  *
  * @retval None
  */
void Neopixel::clear() {
	this->set(0, myledcounts);
}




/**
  * @brief Schalte LEDs in übergebener Farbe an
  * @param  colors: Pointer auf Array,
  * Farben: je 8 Bit Gruen,Rot,Blau;  High bit first.
  * Erstes Element: Erste LED.
  * @retval None
  */
void Neopixel::setcolor(uint32_t* colors)
{
	for(int j = 0; j<(this->myledcounts); j++) // für jede LED einmal durch
	{
		for(int k=0; k<24; k++) //jeden Farbwert einmal durch
		{
			myGPIOx->BSRR = myGPIO_Pin;	// Pin High
			if(colors[j] & (1<<k ))	{w810nop }
			myGPIOx->BSRR = (uint32_t)myGPIO_Pin << 16u;	// Pin Low

			if(!(colors[j] & (1<<k ))) { w810nop }
		}
	}
	myGPIOx->BSRR = (uint32_t)myGPIO_Pin << 16u;
	for(int k = 0; k < 199; k++){ __asm__ __volatile__ ("nop");	}
}


uint8_t Neopixel::getledcount(){
	return this->myledcounts;
} 

#pragma GCC pop_options