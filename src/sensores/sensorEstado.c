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
extern unsigned long g_ul_keypad_switches; /*Valor leído en los botones*/
extern unsigned char g_ucChangedData; /*Si ha cambiado la tecla que se está pulsando*/
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
tBoolean BROADCAR_TR_cambio_estado(void){
	tBoolean cambio = false; /*Si ha cambiado el estado de la tecla*/

	if(g_ucChangedData && g_ucChangedData < 10){
		if(g_ul_keypad_switches == KEY_UP){
			cambio = true;
		}
	}

	return cambio;
}
/**
 * @brief  Funcion que mira el estado del sensor del estado de la carretera
*/
void BROADCAR_ACCION_estado_carretera(void){
	unsigned char * pantalla = malloc(sizeof(unsigned char) * 20);

	if((g_sc_sensores[SENSOR_ESTADO].hora + 10) < g_i_hora){
		g_sc_sensores[SENSOR_ESTADO].tipo = LIQUIDO_CARRETERA;
		g_sc_sensores[SENSOR_ESTADO].hora = g_i_hora;
		g_sc_sensores[SENSOR_ESTADO].posicion.latitud = NORTE;
		g_sc_sensores[SENSOR_ESTADO].posicion.latitud_grado = 90;
		g_sc_sensores[SENSOR_ESTADO].posicion.latitud_minuto = 59;
		g_sc_sensores[SENSOR_ESTADO].posicion.latitud_segundo = 58;
		g_sc_sensores[SENSOR_ESTADO].posicion.longitud = OESTE;
		g_sc_sensores[SENSOR_ESTADO].posicion.longitud_grado = 180;
		g_sc_sensores[SENSOR_ESTADO].posicion.longitud_minuto = 2;
		g_sc_sensores[SENSOR_ESTADO].posicion.longitud_segundo = 3;
		g_sc_sensores[SENSOR_ESTADO].valor = 15;

		BROADCAR_enviar_mensaje(g_sc_sensores[SENSOR_ESTADO]);
		sprintf(pantalla, "aceite %d", g_sc_sensores[SENSOR_ESTADO].hora);
		BROADCAR_escribir(pantalla);
	}else{
		strcpy(pantalla, "no aceite");
		BROADCAR_escribir(pantalla);
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
