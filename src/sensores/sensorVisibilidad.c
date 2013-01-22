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
#include "hw_types.h"
#include "broadcar.h"
#include "display.h"
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
 * visibilidad
 *
 * @return        Si el cambio se ha producido en el sensor de visibilidad
*/
tBoolean BROADCAR_TR_cambio_visibilidad(void){
	tBoolean cambio = false;

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
	BROADCAR_escribir("niebla");
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
