#include "farbspiel.h"

#include "display.h"
#include "neopixel.h"
//#include "gesturesens.h"
#include "gameconfig.h"
#include "Adafruit_APDS9960.h"

/*
"Farbspiel"
Display schreibt "wunsch" nach einem bestimmten essen.
Entsrpechend muss eine Farbe, die dem essen entspricht vor den sensor gehalten werden ( ggf mit taschenlampe, damit die helligkeit ausreicht)
 */



    Farbspiel::Farbspiel(Adafruit_APDS9960 * gesturesenss, Neopixel * neopixel, Display * display, Gameconfig * gameconfig) : Spiel( std::string("Farbspiel") )
    {
    fsneopixel = neopixel;
    fsoleddisplay = display;
    fsgameconfig = gameconfig;  
    fsgesturesens = gesturesenss;
    }


    const char startmsg[]= "Ich habe hunger!";
    const char redmsg[]= "Geb mir eine Erdbeere!";
    const char greenmsg[]= "Geb mir Salat!";
    const char bluemsg[]= "Geb mir Heidelbeeren!";
    const char satt[]= "danke, ich bin satt"; 
    uint16_t rotrel, gruenrel, blaurel; 

    
    void Farbspiel::run()
    {
        
    fsgesturesens->getColorData(&initrot, &initgruen, &initblau, &initfarbtemp);
    fsoleddisplay->cleanbuffer();
    fsoleddisplay->writestring(0,10,startmsg); //"Finde die richtige Farbe!"
    fsoleddisplay->updatescreen();
    HAL_Delay(3000);
    fsoleddisplay->cleanbuffer();
    fsoleddisplay->writestring(0,20,redmsg); //"Finde die richtige Farbe!"
    fsoleddisplay->updatescreen();
    while(1)
    {
        fsgesturesens->getColorData(&rot, &gruen, &blau, &farbtemp); 
         
        rotrel = rot/(initrot+1);   //+1 um keine div/0 zu haben
        gruenrel= gruen/(initgruen+1);
        blaurel= blau/(initblau+1); 
        
         //   if(((rot-initrot)>10 ) && ((gruen-initgruen)<10) && ((blau-initblau)< 10) )
        if(rotrel > ((gruenrel+blaurel))) 
        {
            break;    
        }
        HAL_Delay(100);
    }
    
    fsoleddisplay->cleanbuffer();
    fsoleddisplay->writestring(0,20,greenmsg); //"Finde die richtige Farbe!"
    fsoleddisplay->updatescreen();
    HAL_Delay(3000);
    while(1)
    {
        fsgesturesens->getColorData(&rot, &gruen, &blau, &farbtemp); 
       
        rotrel = rot/(initrot+1);   //+1 um keine div/0 zu haben
        gruenrel= gruen/(initgruen+1);
        blaurel= blau/(initblau+1); 
        
         //   if(((rot-initrot)>10 ) && ((gruen-initgruen)<10) && ((blau-initblau)< 10) )
        if(gruenrel  > ((rotrel+(blaurel/2)))) 
        {
            break;    
        }
        HAL_Delay(100);
    }
    
    fsoleddisplay->cleanbuffer();
    fsoleddisplay->writestring(0,20,bluemsg); //"Finde die richtige Farbe!"
    fsoleddisplay->updatescreen();
    HAL_Delay(3000);
    while(1)
    {
        fsgesturesens->getColorData(&rot, &gruen, &blau, &farbtemp); 
        
        rotrel = rot/(initrot+1);   //+1 um keine div/0 zu haben
        gruenrel= gruen/(initgruen+1);
        blaurel= blau/(initblau+1); 
        
         //   if(((rot-initrot)>10 ) && ((gruen-initgruen)<10) && ((blau-initblau)< 10) )
        if(blaurel > ((rotrel+gruenrel))) 
        {
            break;    
        }
        HAL_Delay(100);        
    }

    fsoleddisplay->cleanbuffer();
    fsoleddisplay->writestring(0,10,satt); //"Finde die richtige Farbe!"
    fsoleddisplay->updatescreen();
    HAL_Delay(3000);

    


}