#ifndef TEMPSPIEL_H
#define TEMPSPIEL_H


#include "spiel.h"

class Tempsens;
class Display;
class Gameconfig;  


class Tempspiel : public Spiel
{
private:
    Tempsens * tstempsens;
    Display * tsoleddisplay;
    Gameconfig * tsgameconfig;
    float temp;
    

public:
    Tempspiel(Tempsens * tempsensor, Display * display, Gameconfig * gameconfig);
   // ~Tempspiel();
    void run(void) override ; //info, das Compiler weiß, dass run überschrieben werden soll 
};






#endif