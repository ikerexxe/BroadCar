/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : sensorVisibilidad.c									**
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
#define SENSORVISIBILIDAD_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
//TODO: no hay
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Funcion que mira si el sensor que ha cambiado de valor es el
 * visibilidad
 *
 * @return        Si el cambio se ha producido en el sensor de visibilidad
*/
tBoolean BROADCAR_TR_cambio_visibilidad(void){
	tBoolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_ucChangedData && g_ucChangedData < 10){
		if(g_ul_keypad_switches == KEY_LEFT){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor de visibilidad
*/
void BROADCAR_ACCION_poca_visibilidad(void){
	unsigned char * pantalla = malloc(sizeof(unsigned char) * 20);

	if((g_sc_sensores[SENSOR_VISIBILIDAD].hora + 10) < g_i_hora){
		g_sc_sensores[SENSOR_VISIBILIDAD].tipo = LUMINOSIDAD;
		g_sc_sensores[SENSOR_VISIBILIDAD].hora = g_i_hora;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.latitud = NORTE;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.latitud_grado = 90;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.latitud_minuto = 59;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.latitud_segundo = 58;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.longitud = OESTE;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.longitud_grado = 180;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.longitud_minuto = 2;
		g_sc_sensores[SENSOR_VISIBILIDAD].posicion.longitud_segundo = 3;
		g_sc_sensores[SENSOR_VISIBILIDAD].valor = 33;

		BROADCAR_enviar_mensaje(g_sc_sensores[SENSOR_VISIBILIDAD]);
		sprintf(pantalla, "niebla %d", g_sc_sensores[SENSOR_VISIBILIDAD].hora);
		BROADCAR_escribir(pantalla);
	}else{
		strcpy(pantalla, "no niebla");
		BROADCAR_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
