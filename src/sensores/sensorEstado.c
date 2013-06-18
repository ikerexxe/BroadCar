/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : sensorEstado.c										**
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
#define SENSORESTADO_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "sensorEstado.h"
#include "broadcar.h"
#include "data.h"
#include "zigbee.h"
#include "bluetooth.h"
#ifdef STELLARIS
#include "displayStellaris.h"
#else
#include "displaySpartan.h"
#endif
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Funcion que mira si el sensor que ha cambiado de valor es el
 * del estado de la carretera
 *
 * @return        Si el cambio se ha producido en el sensor de estado
 * de la carretera
*/
boolean S_ESTADO_cambio(void){
	boolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_uc_changed_data && g_uc_changed_data < 10){
		if(g_ul_keypad_switches == KEY_UP){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor del estado de la carretera
*/
void S_ESTADO_accion(void){
	unsigned char * pantalla;
	MENSAJEClass mensaje;

	if((g_cs_sensores[SENSOR_ESTADO].hora + 0) < g_i_hora){
		pantalla = malloc(sizeof(unsigned char) * 20);

		g_cs_sensores[SENSOR_ESTADO].tipo = LIQUIDO_CARRETERA;
		g_cs_sensores[SENSOR_ESTADO].hora = g_i_hora;
		g_cs_sensores[SENSOR_ESTADO].posicion.latitud = NORTE;
		g_cs_sensores[SENSOR_ESTADO].posicion.latitud_grado = 43;
		g_cs_sensores[SENSOR_ESTADO].posicion.latitud_minuto = 3;
		g_cs_sensores[SENSOR_ESTADO].posicion.latitud_segundo = 46;
		g_cs_sensores[SENSOR_ESTADO].posicion.longitud = OESTE;
		g_cs_sensores[SENSOR_ESTADO].posicion.longitud_grado = 2;
		g_cs_sensores[SENSOR_ESTADO].posicion.longitud_minuto = 30;
		g_cs_sensores[SENSOR_ESTADO].posicion.longitud_segundo = 23;
		g_cs_sensores[SENSOR_ESTADO].valor = 15;

		mensaje = ZIGBEE_crear_mensaje(g_cs_sensores[SENSOR_ESTADO]);
		ZIGBEE_enviar_mensaje(mensaje);
		BLUETOOTH_enviar_mensaje(mensaje);
		sprintf(pantalla, "aceite %d", g_cs_sensores[SENSOR_ESTADO].hora);
		DISPLAY_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
