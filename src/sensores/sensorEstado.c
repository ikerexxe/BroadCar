/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : sensorEstado.c									**
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
	SENSORClass sensor; /*Contiene los datos del sensor*/

	sensor.tipo = LIQUIDO_CARRETERA;
	sensor.hora = 10;
	sensor.posicion.latitud = NORTE;
	sensor.posicion.latitud_grado = 90;
	sensor.posicion.latitud_minuto = 59;
	sensor.posicion.latitud_segundo = 58;
	sensor.posicion.longitud = OESTE;
	sensor.posicion.longitud_grado = 180;
	sensor.posicion.longitud_minuto = 2;
	sensor.posicion.longitud_segundo = 3;
	sensor.valor = 15;

	BROADCAR_enviar_mensaje(sensor);
	BROADCAR_escribir("hielo");
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
