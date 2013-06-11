/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : main.c 												**
** version : 1 														**
** date : 2013-01-17	 											**
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
**Date : 2013-01-17													**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "hw_types.h"
#include "broadcar.h"
#include "clock.h"
#include "display.h"
#include "keypad.h"
#include "zigbee.h"
#include "bluetooth.h"
#include "sensorEstado.h"
#include "sensorObras.h"
#include "sensorVelocidad.h"
#include "sensorVisibilidad.h"
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void BROADCAR_inicializacion();
void BROADCAR_logica();
void BROADCAR_inicializacion_sensores();
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
int main(void)
{
	BROADCAR_inicializacion();

    while(1){
    	BROADCAR_logica();
    }
}
/**
 * @brief  Función para inicialización de los periféricos.
 *
 * @return    -
 *
 * Se inicializan uno a uno el clock, los sensores, los mensajes, el keypad,
 * la pantalla y la comunicacion zigbee.
*/
void BROADCAR_inicializacion(){
	g_i_numero_mensaje = 0;
	g_i_mi_id = 2;
	CLOCK_inicializacion();
	BROADCAR_inicializacion_sensores();
	KEYPAD_inicializacion();
	DISPLAY_inicializacion();
	ZIGBEE_inicializacion();
	BLUETOOTH_inicializacion();
}
/**
 * @brief  Función donde se ejecuta la logica principal del programa.
 *
 * @return    -
 *
 * Primero se lee la tecla pulsada, luego se mira cual ha sido la tecla pulsada
 * y se actua en consecuencia, es decir, se muestra en pantalla la alerta y se envia
 * por zigbee y bluetooth. Finalmente se mira si se ha recibido algun mensaje de
 * otro controlador o del movil.
*/
void BROADCAR_logica(){
	KEYPAD_leer();

	if(S_ESTADO_cambio()){
		S_ESTADO_accion();
	}else if(S_OBRAS_cambio()){
		S_OBRAS_accion();
	}else if(S_VELOCIDAD_cambio()){
		S_VELOCIDAD_accion();
	}else if(S_VISIBILIDAD_cambio()){
		S_VISIBILIDAD_accion();
	}

	ZIGBEE_recepcion_mensajes();
	BLUETOOTH_recepcion_mensajes();
}
/**
 * @brief  Función que inicializa el array que contiene el valor de los sensores.
 *
 * @return    -
 *
 * Se inicializa el array que contiene el valor de los sensores de lo que
 * dispone el vehiculo.
*/
void BROADCAR_inicializacion_sensores(){
	int contador_sensores = 0;

	g_cs_sensores[0].tipo = LUMINOSIDAD;
	g_cs_sensores[1].tipo = LIQUIDO_CARRETERA;
	g_cs_sensores[2].tipo = S_OBRAS;
	g_cs_sensores[3].tipo = VELOCIDAD;

	for(contador_sensores = 0; contador_sensores < NUMERO_SENSORES; contador_sensores++){
		g_cs_sensores[contador_sensores].hora = 0;
		g_cs_sensores[contador_sensores].posicion.latitud = 0;
		g_cs_sensores[contador_sensores].posicion.latitud_grado = 0;
		g_cs_sensores[contador_sensores].posicion.latitud_minuto = 0;
		g_cs_sensores[contador_sensores].posicion.latitud_segundo = 0;
		g_cs_sensores[contador_sensores].posicion.longitud = 0;
		g_cs_sensores[contador_sensores].posicion.longitud_grado = 0;
		g_cs_sensores[contador_sensores].posicion.longitud_minuto = 0;
		g_cs_sensores[contador_sensores].posicion.longitud_segundo = 0;
		g_cs_sensores[contador_sensores].valor = 0;
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
