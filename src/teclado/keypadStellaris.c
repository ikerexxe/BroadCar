/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : keypad.c 												**
** version : 1 														**
** date : 2012-10-08	 											**
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
**Version : 3														**
**Date : 2013-01-17													**
**Revised by : iker pedrosa											**
**Description : Modified version for broadcar application.			**
*********************************************************************/
#define KEYPAD_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "hw_types.h"
#include "hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "broadcar.h"
#include "keypadStellaris.h"
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Inicializamos el teclado (los botones).
 *
 * @return      -
 *
*/
void KEYPAD_inicializacion(){
	//Activamos pines del puerto F (bot�n select)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOInput( GPIO_PORTF_BASE, GPIO_PIN_1); /*pin 1*/
	GPIOPadConfigSet( GPIO_PORTF_BASE, GPIO_PIN_1 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU);
	//Activamos pines del puerto E (botones up, down, right, left)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOInput( GPIO_PORTE_BASE, GPIO_PIN_0); /*pin 0*/
	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_0 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput( GPIO_PORTE_BASE, GPIO_PIN_1); /*pin 1*/
	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_1 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput( GPIO_PORTE_BASE, GPIO_PIN_2); /*pin 2*/
	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_2 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput( GPIO_PORTE_BASE, GPIO_PIN_3); /*pin 3*/
	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_3 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU);
}
/**
 * @brief  Leemos el bot�n pulsado en el teclado.
 *
 * @return      -
 *
*/
void KEYPAD_leer(){
	unsigned long ul_pressed_data; /*Guarda el valor de la tecla pulsada*/
	ul_pressed_data = (GPIOPinRead( GPIO_PORTE_BASE , (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3) )|
				    	(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) << 3));
	g_uc_changed_data = g_ul_keypad_switches ^ ul_pressed_data;
	g_ul_keypad_switches = ul_pressed_data;
	g_ul_keypad_switches = g_ul_keypad_switches & 0x1f;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/







