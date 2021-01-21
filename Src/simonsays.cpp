#include "simonsays.h"
#include "neopixel.h"
#include "display.h"
#include "buttonmatrix.h"
#include "gameconfig.h"
#include <cstdlib>


#pragma GCC push_options
#pragma GCC optimize ("Og") //optimierung für debugging


const uint32_t farbtabelle[4] = {color_rot,color_gruen, color_blau,color_weiss};

/*
"Simon says"
es wird eine Farb-&position vorgegeben, die der Spieler anschließend mit den Tastern nachmachen muss.
für 4 LEDs mit 4 möglichen Farben: Rot, Grün, Blau Weiß
Die Schwierigkeit, also die anzahl der Farben in die die LEDs aufleuchen kann eingestellt werden.
Ebenso die rundenzahl. 

*/


Simonsays::Simonsays(Neopixel * neopixel, Display * display,  Gameconfig * gameconfig) : Spiel( std::string("Simonsays") )
{
    ssneopixel= neopixel;
    ssoleddisplay = display;
    ssgameconfig= gameconfig;
    button = 0; 
    ledcount = ssneopixel->getledcount();
    ledanzeige = new uint32_t[ledcount];
}

Simonsays::~Simonsays()
{
    delete[] ledanzeige;
}




uint8_t ledposition[256]; 
uint8_t ledfarbe[256]; 
void Simonsays::run()
{
    ssoleddisplay->setfont(2);
    ssoleddisplay->cleanbuffer();
    const char mychar[]= "Simon Says";
    ssoleddisplay->writestring(0,20,mychar);
    ssoleddisplay->updatescreen();




    
    int rundenanzahl= ssgameconfig->get("ssrunden");   //"ssrunden"
    if ( rundenanzahl >255) rundenanzahl = 255; 
    int farbanzahl = ssgameconfig->get("ssfarben");
    if ( farbanzahl > 4) farbanzahl = 4;
    if ( farbanzahl ==0) farbanzahl = 1;
    int failcounter =0;
    volatile bool gamefailed=false;

    for (  uint8_t runde = 0; runde < rundenanzahl  ;  runde++) // Zufallsreihenfolge generieren 
    {
            ledposition[runde]= (std::rand() % 4);  // zufallswert 0...3 
            ledfarbe[runde]= (std::rand() % farbanzahl); //zufallsfarbe
    }

    for (uint8_t runde = 0; runde < rundenanzahl;  runde++)  //ganze Spiel 
    {
        if(gamefailed) // falsche Taste gedrückt, spiel neustarten 
        {
            gamefailed=false;
            runde=0;
            failcounter++; 
        } 
        if(failcounter >5) //Tipp beim 6. mal fehlschlagen anzeigen
        {
            const char mychar2[]= "Tipp: RGBW";
            ssoleddisplay->writestring(0,40,mychar2);
            ssoleddisplay->updatescreen();
        }

        for (uint8_t j = 0; j < runde+1; j++) // LEDs anzeigen (eine runde)
        {    
            memset(this->ledanzeige, 0x00000000, this->ledcount*sizeof(uint32_t));
            
            ledanzeige[ledposition[j]] = farbtabelle[ledfarbe[j]]; 
            ssneopixel->setcolor(ledanzeige);
            HAL_Delay(1000);
            ssneopixel->clear(); 
            HAL_Delay(500);
        }    
            
        for (uint8_t j = 0; j < runde+1; j++) // Vergleich ob richtig gedrückt (eine Runde)
        {    
            button=0;
            while(button==0) //warten bis taster gedrückt 
            {             
                button=getButton();
            }
            
            if(((button-1)%4)==ledposition[j] && ((button-1)/4)==ledfarbe[j]) // wenn Taster richtig gedrückt
            {
                memset(this->ledanzeige, 0x00000000, this->ledcount*sizeof(uint32_t));
                ledanzeige[ledposition[j]] = farbtabelle[ledfarbe[j]]; 
                ssneopixel->setcolor(ledanzeige);
                HAL_Delay(800);
                ssneopixel->clear();  
                
            }
            else //gamefailed
            {
                gamefailed=true;
                for (uint8_t k = 0; k < this->ledcount; k++){ ledanzeige[k]= 0x00A000;}
                                
                ssneopixel->setcolor(ledanzeige);
                HAL_Delay(500);
                ssneopixel->clear();
                HAL_Delay(500);
                ssneopixel->setcolor(ledanzeige);
                HAL_Delay(500);
                ssneopixel->clear();
                HAL_Delay(1);
                break;
                
            }
            while(button!=0) //warten bis taster losgelassen 
            {             
                button=getButton();
            }
              
        }
        if(!gamefailed) // Runde erledigt, blinke grün 
        {
            for (uint8_t k = 0; k < this->ledcount; k++){ ledanzeige[k]= 0x0000A0;}
            ssneopixel->setcolor(ledanzeige);
            HAL_Delay(1000);
            ssneopixel->clear();
            HAL_Delay(500);
        }
        
    }


//ssneopixel->set(1,6);
//HAL_Delay (1000);

//ssneopixel->set(0,6);
//HAL_Delay (1000);


//button= getButton();
ssoleddisplay->cleanbuffer();
const char mychar3[]= "Geschafft!";
ssoleddisplay->writestring(0,20,mychar3);
ssoleddisplay->updatescreen();
HAL_Delay(4000);     
}


#pragma GCC pop_options