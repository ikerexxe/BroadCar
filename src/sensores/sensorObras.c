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
#include <stdlib.h>
#include <stdio.h>
#include "hw_types.h"
#include "sensorObras.h"
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
 * aviso de obras
 *
 * @return        Si el cambio se ha producido en el sensor de aviso
 * de obras
*/
tBoolean S_OBRAS_cambio(void){
	tBoolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_uc_changed_data && g_uc_changed_data < 10){
		if(g_ul_keypad_switches == KEY_RIGHT){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor de obras
*/
void S_OBRAS_accion(void){
	unsigned char * pantalla;
	MENSAJEClass mensaje;

	if((g_cs_sensores[SENSOR_OBRAS].hora + 0) < g_i_hora){
		pantalla = malloc(sizeof(unsigned char) * 20);

		g_cs_sensores[SENSOR_OBRAS].tipo = S_OBRAS;
		g_cs_sensores[SENSOR_OBRAS].hora = g_i_hora;
		g_cs_sensores[SENSOR_OBRAS].posicion.latitud = NORTE;
		g_cs_sensores[SENSOR_OBRAS].posicion.latitud_grado = 43;
		g_cs_sensores[SENSOR_OBRAS].posicion.latitud_minuto = 3;
		g_cs_sensores[SENSOR_OBRAS].posicion.latitud_segundo = 46;
		g_cs_sensores[SENSOR_OBRAS].posicion.longitud = OESTE;
		g_cs_sensores[SENSOR_OBRAS].posicion.longitud_grado = 2;
		g_cs_sensores[SENSOR_OBRAS].posicion.longitud_minuto = 30;
		g_cs_sensores[SENSOR_OBRAS].posicion.longitud_segundo = 23;
		g_cs_sensores[SENSOR_OBRAS].valor = 5;

		mensaje = ZIGBEE_crear_mensaje(g_cs_sensores[SENSOR_OBRAS]);
		ZIGBEE_enviar_mensaje(mensaje);
		BLUETOOTH_enviar_mensaje(mensaje);
		sprintf(pantalla, "obras %d", g_cs_sensores[SENSOR_OBRAS].hora);
		DISPLAY_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
