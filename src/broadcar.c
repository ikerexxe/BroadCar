/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : main.c 												**
** version : 1 														**
** date : 2013-01-17	 											**
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
**Date : 2013-01-17													**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/

/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "broadcar.h"
#include "framebuffer.h"
#include "clock.h"
#include "displayGenerico.h"
#include "display.h"
#include "keypad.h"
#include "hw_types.h"

/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void BROADCAR_inicializacion();
void BROADCAR_logica();

/*********************************************************************
** 																	**
** EXTERNAL VARIABLES 												**
** 																	**
**********************************************************************/
extern unsigned long g_ul_keypad_switches; /*Valor leído en los botones*/
extern unsigned char g_ucChangedData; /*Si ha cambiado la tecla que se está pulsando*/

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
unsigned long g_ul_system_clock; /*Frecuencia del clock*/

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
int main(void)
{
	int i = 0;

	BROADCAR_inicializacion();

    while(1){
    	BROADCAR_logica();
    }
}

/**
 * @brief  Función para inicialización de los periféricos.
 *
 * @return    -
 *
 * Se inicializan uno a uno el clock, el keypad, la pantalla, el PWM y
 * la UART.
*/
void BROADCAR_inicializacion(){
	BROADCAR_inicializacion_clock();
	BROADCAR_inicializacion_keypad();
	BROADCAR_inicializacion_display();
//	CHAT_inicializacion_pwm();
//	CHAT_inicializacion_comunicacion();
}

/**
 * @brief  Función para elegir el paso a seguir en cada momento.
 *
 * @return    -
 *
 * Primero se lee la tecla pulsada, luego se actualiza la pantalla teniendo
 * en cuenta la tecla pulsa, si se pulsa el select se envía el mensaje y se
 * reinicializa la parte de la pantalla en la que escribe el usuario.
 * Si se ha reibido algo por la UART y está almacenado en el buffer de software
 * se muestra en pantalla.
*/
void BROADCAR_logica(){
	unsigned char * pantalla;

	BROADCAR_leer_keypad();
	if(g_ucChangedData){
		pantalla = malloc(sizeof(unsigned char) * MAX_ELEMS_LINEA);
		switch(g_ul_keypad_switches){
			case KEY_UP:
				strcpy(pantalla, "ARRIBA");
				break;
			case KEY_DOWN:
				strcpy(pantalla, "ABAJO");
				break;
			case KEY_LEFT:
				strcpy(pantalla, "IZQUIERDA");
				break;
			case KEY_RIGHT:
				strcpy(pantalla, "DERECHA");
				break;
			case KEY_SELECT:
				strcpy(pantalla, "SELECT");
				break;
			default:
				strcpy(pantalla, "NADA");
				break;
		}
		BROADCAR_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
