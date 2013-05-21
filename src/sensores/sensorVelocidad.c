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
#include "displayStellaris.h"
#include "data.h"
#include "zigbee.h"
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
tBoolean S_VELOCIDAD_cambio(void){
	tBoolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_uc_changed_data && g_uc_changed_data < 10){
		if(g_ul_keypad_switches == KEY_DOWN){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor de velocidad
*/
void S_VELOCIDAD_accion(void){
	unsigned char * pantalla;

	if((g_cs_sensores[SENSOR_VELOCIDAD].hora + 0) < g_i_hora){
		pantalla = malloc(sizeof(unsigned char) * 20);

		g_cs_sensores[SENSOR_VELOCIDAD].tipo = VELOCIDAD;
		g_cs_sensores[SENSOR_VELOCIDAD].hora = g_i_hora;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.latitud = NORTE;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.latitud_grado = 90;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.latitud_minuto = 59;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.latitud_segundo = 58;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.longitud = OESTE;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.longitud_grado = 180;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.longitud_minuto = 2;
		g_cs_sensores[SENSOR_VELOCIDAD].posicion.longitud_segundo = 3;
		g_cs_sensores[SENSOR_VELOCIDAD].valor = 15;

		ZIGBEE_enviar_mensaje(g_cs_sensores[SENSOR_VELOCIDAD]);
		sprintf(pantalla, "lento %d", g_cs_sensores[SENSOR_VELOCIDAD].hora);
		DISPLAY_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
