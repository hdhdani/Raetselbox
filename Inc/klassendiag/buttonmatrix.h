/*
 * buttonmatrix.h
 *
 *  Created on: Dec 15, 2020
 *      Author: Melzer-PC
 */

#ifndef INC_BUTTONMATRIX_H_
#define INC_BUTTONMATRIX_H_


#include "inttypes.h"

/**
 * @brief Lese Taster aus 
 *  * 
 * @return uint8_t  Zahl des gedrueckten Tasters oder 0 bei nichts gedrückt
 */
uint8_t getButton();


#endif /* INC_BUTTONMATRIX_H_ */
