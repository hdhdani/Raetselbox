#ifndef CONFIGCHANGE_H
#define CONFIGCHANGE_H


#include "main.h"



class Display;
class Gameconfig; 



void configchange( UART_HandleTypeDef huart2, Gameconfig * mygameconfig, Display * oleddisplay );



#endif