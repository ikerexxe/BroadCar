/*
 * keypadSpartan.c
 *
 *  Created on: 27/05/2013
 *      Author: Daniel
 */

#include "xparameters.h"
#include "xgpio.h"
#include "xgpio_l.h"
#include "broadcar.h"

extern int DIP_Read;

void KEYPAD_inicializacion(){

}

void KEYPAD_leer()
{
	unsigned long ul_pressed_data; /*Guarda el valor de la tecla pulsada*/

	ul_pressed_data = XGpio_ReadReg(XPAR_DIP_SWITCHES_4BITS_BASEADDR,0);
	g_uc_changed_data = g_ul_keypad_switches ^ ul_pressed_data;
	g_ul_keypad_switches = ul_pressed_data;
	g_ul_keypad_switches = g_ul_keypad_switches;// & 0x1f;

}
