/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : sensorVelocidad.c										**
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
#define SENSORVELOCIDAD_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "hw_types.h"
#include "sensorVelocidad.h"
#include "broadcar.h"
#include "display.h"
#include "data.h"
#include "zigbee.h"
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
//TODO: no hay
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Funcion que mira si el sensor que ha cambiado de valor es el
 * velocidad
 *
 * @return        Si el cambio se ha producido en el sensor de velocidad
*/
tBoolean BROADCAR_TR_cambio_velocidad(void){
	tBoolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_ucChangedData && g_ucChangedData < 10){
		if(g_ul_keypad_switches == KEY_DOWN){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor de velocidad
*/
void BROADCAR_ACCION_velocidad(void){
	unsigned char * pantalla;

	if((g_sc_sensores[SENSOR_VELOCIDAD].hora + 10) < g_i_hora){
		pantalla = malloc(sizeof(unsigned char) * 20);

		g_sc_sensores[SENSOR_VELOCIDAD].tipo = VELOCIDAD;
		g_sc_sensores[SENSOR_VELOCIDAD].hora = g_i_hora;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.latitud = NORTE;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.latitud_grado = 90;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.latitud_minuto = 59;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.latitud_segundo = 58;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.longitud = OESTE;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.longitud_grado = 180;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.longitud_minuto = 2;
		g_sc_sensores[SENSOR_VELOCIDAD].posicion.longitud_segundo = 3;
		g_sc_sensores[SENSOR_VELOCIDAD].valor = 15;

		BROADCAR_enviar_mensaje(g_sc_sensores[SENSOR_VELOCIDAD]);
		sprintf(pantalla, "lento %d", g_sc_sensores[SENSOR_VELOCIDAD].hora);
		BROADCAR_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
