

#include "tempspiel.h"
#include "tempsens.h"
#include "display.h"
#include "buttonmatrix.h"
#include "gameconfig.h"
#include <cstdlib>


Tempspiel::Tempspiel(Tempsens * tempsensor, Display * display, Gameconfig * gameconfig) : Spiel( std::string("Simonsays") )
//Simonsays::Simonsays(Neopixel * neopixel, Display * display,  Gameconfig * gameconfig) : Spiel( std::string("Simonsays") )
{
    tstempsens= tempsensor;
    tsoleddisplay= display;
    tsgameconfig= gameconfig;
}

const char msgcold1[]= "Erhöhe die";
const char msgcold2[]="Temperatur!"; 
const char msgok[]= "Sehr Gut!";
char isttemparr[7]; 
const char temperr[]="Temperaturfehler";

void Tempspiel::run()
{
    
    tsoleddisplay->cleanbuffer();
    tsoleddisplay->updatescreen(); //Display leeren 
    tsoleddisplay->writestring(0,5,msgcold1);
    tsoleddisplay->writestring(0,15,msgcold2);
    tsoleddisplay->updatescreen(); //Schreibe aufgabe
    HAL_Delay(3000); 
    temp= tstempsens->readtemp();
    if(temp ==0)
    {
        tsoleddisplay->cleanbuffer();
        tsoleddisplay->updatescreen(); //Display leeren 
        tsoleddisplay->writestring(0,5,temperr);
        tsoleddisplay->updatescreen();
        HAL_Delay(3000);
        return;
    }
    
    while ((temp +2)> tstempsens->readtemp())
    {
        HAL_Delay(100); // etwas warten...
        tsoleddisplay->cleanbuffer();
        float isttemp = tstempsens->readtemp();
        sprintf(isttemparr, "%2.2fC", isttemp); 
        tsoleddisplay->writestring(0,5,isttemparr);
        tsoleddisplay->updatescreen(); //Display leeren 
    }

    tsoleddisplay->cleanbuffer();
    tsoleddisplay->writestring(0,5,msgok);
    tsoleddisplay->updatescreen();
    HAL_Delay(2000); //Rätsel gelöst fertig 

}