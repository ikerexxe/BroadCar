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
#include "framebuffer.h"
#include "clock.h"
#include "displayGenerico.h"
#include "display.h"
#include "hw_types.h"

/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void CHAT_inicializacion();
void CHAT_logica();

//TODO: esto va en el header
#define MAX_ELEMS_LINEA 10

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
unsigned long g_ul_system_clock; /*Frecuencia del clock*/
unsigned char uno = 97;
unsigned char dos = 98;

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
int main(void)
{
	int i = 0;

	CHAT_inicializacion();

    while(1){
    	CHAT_logica();
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
void CHAT_inicializacion(){
	CHAT_inicializacion_clock();
//	CHAT_inicializacion_keypad();
	CHAT_inicializacion_display();
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
void CHAT_logica(){
	unsigned char * pantalla = malloc(sizeof(unsigned char) * MAX_ELEMS_LINEA);

	pantalla[0] = uno;
	pantalla[1] = dos;
	pantalla[2] = '\0';
	uno++;
	dos++;

	CHAT_escribir(pantalla);
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
