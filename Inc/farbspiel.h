#ifndef FARBSPIEL_H
#define FARBSPIEL_H

#include "spiel.h"

class Neopixel;
class Display;
class Gameconfig;  
class Adafruit_APDS9960;


class Farbspiel  : public Spiel
{
private:
    Neopixel * fsneopixel;
    Display * fsoleddisplay;
    Gameconfig * fsgameconfig;  
    Adafruit_APDS9960 * fsgesturesens;
    uint16_t gruen, rot, blau, farbtemp;
    uint16_t initgruen, initrot, initblau, initfarbtemp;
     


public:
    Farbspiel(Adafruit_APDS9960 * gesturesenss, Neopixel * neopixel, Display * display, Gameconfig *gameconfig);
    //~Farbspiel();
    void run(void) override ; // override= info, das Compiler weiß, dass run überschrieben werden soll 

};


#endif