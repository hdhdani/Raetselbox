#ifndef SIMONSAYS_H
#define SIMONSAYS_H


#include "spiel.h"
class Neopixel;
class Display;
class Gameconfig;  

class Simonsays : public Spiel
{
private:
    Neopixel * ssneopixel;
    Display * ssoleddisplay;
    Gameconfig * ssgameconfig;
    uint8_t button;
    uint8_t runden; 
    uint8_t farbanzahl;
    uint32_t  * ledanzeige;
    uint32_t ledcount;
   

    
public:
    Simonsays(Neopixel * neopixel, Display * display, Gameconfig * gameconfig);
    ~Simonsays();
    void run(void) override ; //info, das Compiler weiß, dass run überschrieben werden soll 

};



#endif