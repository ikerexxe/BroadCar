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
#include "hw_types.h"
#include "broadcar.h"
#include "clock.h"
#include "display.h"
#include "keypad.h"
#include "motorAutomatas.h"
#include "zigbee.h"
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void BROADCAR_inicializacion();
void BROADCAR_logica();
void BROADCAR_inicializacion_sensores();
void BROADCAR_inicializacion_mensajes();
/*********************************************************************
** 																	**
** EXTERNAL VARIABLES 												**
** 																	**
**********************************************************************/
extern TS_AUTOMATA automata; /*Automata que usa la aplicacion*/
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
//TODO: no hay
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
int main(void)
{
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
 * Se inicializan uno a uno el clock, los sensores, los mensajes, el keypad,
 * la pantalla y la comunicacion zigbee.
*/
void BROADCAR_inicializacion(){
	g_i_numero_mensaje = 0;
	g_i_mi_id = 1;
	BROADCAR_inicializacion_clock();
	BROADCAR_inicializacion_sensores();
	BROADCAR_inicializacion_keypad();
	BROADCAR_inicializacion_display();
	BROADCAR_inicializacion_zigbee();
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
	EjecutaAutomata(&automata);
}
/**
 * @brief  Función que inicializa el array que contiene el valor de los sensores.
 *
 * @return    -
 *
 * Se inicializa el array que contiene el valor de los sensores de lo que
 * dispone el vehiculo.
*/
void BROADCAR_inicializacion_sensores(){
	int contador_sensores = 0;

	g_sc_sensores[0].tipo = LUMINOSIDAD;
	g_sc_sensores[1].tipo = LIQUIDO_CARRETERA;
	g_sc_sensores[2].tipo = S_OBRAS;
	g_sc_sensores[3].tipo = VELOCIDAD;

	for(contador_sensores = 0; contador_sensores < NUMERO_SENSORES; contador_sensores++){
		g_sc_sensores[contador_sensores].hora = 0;
		g_sc_sensores[contador_sensores].posicion.latitud = 0;
		g_sc_sensores[contador_sensores].posicion.latitud_grado = 0;
		g_sc_sensores[contador_sensores].posicion.latitud_minuto = 0;
		g_sc_sensores[contador_sensores].posicion.latitud_segundo = 0;
		g_sc_sensores[contador_sensores].posicion.longitud = 0;
		g_sc_sensores[contador_sensores].posicion.longitud_grado = 0;
		g_sc_sensores[contador_sensores].posicion.longitud_minuto = 0;
		g_sc_sensores[contador_sensores].posicion.longitud_segundo = 0;
		g_sc_sensores[contador_sensores].valor = 0;
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
