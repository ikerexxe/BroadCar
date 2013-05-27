/*
 * keypadSpartan.c
 *
 *  Created on: 27/05/2013
 *      Author: Daniel
 */

#include "xparameters.h"
#include "xgpio.h"
#include "xgpio_l.h"

extern int DIP_Read;

void KEYPAD_leer()
{
	DIP_Read = XGpio_ReadReg(XPAR_DIP_SWITCHES_4BITS_BASEADDR,0);
}
