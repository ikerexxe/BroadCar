/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : keypadSpartan.c										**
** version : 1 														**
** date : 2013-05-27	 											**
** 																	**
**********************************************************************
** 																	**
** Copyright (c) 2012,		 										**
** All rights reserved. 											**
** 																	**
**********************************************************************
**																	**
**VERSION HISTORY:													**
**----------------													**
**Version : 1														**
**Date :  2013-05-27												**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/
#define KEYPADSPARTAN_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "xparameters.h"
#include "xgpio.h"
#include "xgpio_l.h"
#include "broadcar.h"
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
//TODO: donde se usa esta variable?
int DIP_Read;
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
//TODO: a comentar lo que hace esta funcion
void KEYPAD_inicializacion(){

}
//TODO: a comentar lo que hace esta funcion
void KEYPAD_leer()
{
	unsigned long ul_pressed_data; /*Guarda el valor de la tecla pulsada*/

	ul_pressed_data = XGpio_ReadReg(XPAR_DIP_SWITCHES_4BITS_BASEADDR,0);
	g_uc_changed_data = g_ul_keypad_switches ^ ul_pressed_data;
	g_ul_keypad_switches = ul_pressed_data;
	g_ul_keypad_switches = g_ul_keypad_switches;// & 0x1f;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
