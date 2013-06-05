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
#include "hw_types.h"
#include "sensorVisibilidad.h"
#include "broadcar.h"
#include "display.h"
#include "data.h"
#include "zigbee.h"
#include "bluetooth.h"
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
tBoolean S_VISIBILIDAD_cambio(void){
	tBoolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_uc_changed_data && g_uc_changed_data < 10){
		if(g_ul_keypad_switches == KEY_LEFT){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor de visibilidad
*/
void S_VISIBILIDAD_accion(void){
	unsigned char * pantalla;
	MENSAJEClass mensaje;

	if((g_cs_sensores[SENSOR_VISIBILIDAD].hora + 0) < g_i_hora){
		pantalla = malloc(sizeof(unsigned char) * 20);

		g_cs_sensores[SENSOR_VISIBILIDAD].tipo = LUMINOSIDAD;
		g_cs_sensores[SENSOR_VISIBILIDAD].hora = g_i_hora;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.latitud = NORTE;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.latitud_grado = 43;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.latitud_minuto = 3;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.latitud_segundo = 36;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.longitud = OESTE;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.longitud_grado = 2;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.longitud_minuto = 30;
		g_cs_sensores[SENSOR_VISIBILIDAD].posicion.longitud_segundo = 23;
		g_cs_sensores[SENSOR_VISIBILIDAD].valor = 33;

		mensaje = ZIGBEE_crear_mensaje(g_cs_sensores[SENSOR_VISIBILIDAD]);
		ZIGBEE_enviar_mensaje(mensaje);
		BLUETOOTH_enviar_mensaje(mensaje);
		sprintf(pantalla, "niebla %d", g_cs_sensores[SENSOR_VISIBILIDAD].hora);
		DISPLAY_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
