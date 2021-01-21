
#include "configchange.h"

#include "gameconfig.h"
#include "display.h"


void configchange( UART_HandleTypeDef huart2, Gameconfig * mygameconfig, Display * oleddisplay ){

char uartchange[] = "config changed";
uint8_t uartrec[2];
uartrec[0] = 0;
uartrec[1]=0; 

 HAL_UART_Receive(& huart2 , uartrec, 2, 100);

  if(uartrec[0] == 1)
  { 
    mygameconfig->set( "ssrunden", uartrec[1],1);
    
  }
  if(uartrec[0] == 2) 
  { 
    mygameconfig->set( "ssfarben",  uartrec[1],1); 
    
  }
  if(uartrec[0] == 3)
  {
    mygameconfig->set("aswaves",uartrec[1],1);
    
  }
  if(uartrec[0] != 0)
  {
    
    //oleddisplay->cleanbuffer();
    oleddisplay->setfont(1);
    oleddisplay->writestring(0,60,uartchange);
    oleddisplay->updatescreen();
  }


  uartrec[0] = 0;
  uartrec[1]=0; 

}