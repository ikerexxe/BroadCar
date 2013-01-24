/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : sensorObras.c											**
** version : 1 														**
** date : 2013-01-22	 											**
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
**Date : 2013-01-22													**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/
#define SENSOROBRAS_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "hw_types.h"
#include "broadcar.h"
#include "display.h"
#include "data.h"
#include "zigbee.h"
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern unsigned long g_ul_keypad_switches; /*Valor le�do en los botones*/
extern unsigned char g_ucChangedData; /*Si ha cambiado la tecla que se est� pulsando*/
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Funcion que mira si el sensor que ha cambiado de valor es el
 * aviso de obras
 *
 * @return        Si el cambio se ha producido en el sensor de aviso
 * de obras
*/
tBoolean BROADCAR_TR_cambio_obras(void){
	tBoolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_ucChangedData && g_ucChangedData < 10){
		if(g_ul_keypad_switches == KEY_RIGHT){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor de obras
*/
void BROADCAR_ACCION_obras(void){
	if((g_sc_sensores[SENSOR_OBRAS].hora + 10) < g_i_hora){
		g_sc_sensores[SENSOR_OBRAS].tipo = S_OBRAS;
		g_sc_sensores[SENSOR_OBRAS].hora = g_i_hora;
		g_sc_sensores[SENSOR_OBRAS].posicion.latitud = NORTE;
		g_sc_sensores[SENSOR_OBRAS].posicion.latitud_grado = 90;
		g_sc_sensores[SENSOR_OBRAS].posicion.latitud_minuto = 59;
		g_sc_sensores[SENSOR_OBRAS].posicion.latitud_segundo = 58;
		g_sc_sensores[SENSOR_OBRAS].posicion.longitud = OESTE;
		g_sc_sensores[SENSOR_OBRAS].posicion.longitud_grado = 180;
		g_sc_sensores[SENSOR_OBRAS].posicion.longitud_minuto = 2;
		g_sc_sensores[SENSOR_OBRAS].posicion.longitud_segundo = 3;
		g_sc_sensores[SENSOR_OBRAS].valor = 100;

		BROADCAR_enviar_mensaje(g_sc_sensores[SENSOR_OBRAS]);
		BROADCAR_escribir("obras");
	}else{
		BROADCAR_escribir("no obras");
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
