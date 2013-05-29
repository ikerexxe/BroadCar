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
#include "broadcar.h"
#include "motorAutomatas.h"
#include "zigbee.h"
#ifdef STELLARIS
#include "hw_types.h"
#include "clockStellaris.h"
#include "displayStellaris.h"
#include "keypadStellaris.h"
#else
#include "clockSpartan.h"
#include "displaySpartan.h"
#include "keypadSpartan.h"
#include "xintc.h"
#include "xintc_l.h"
#include "xil_exception.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xparameters.h"
#endif
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
** EXTERNAL VARIABLES 												**
** 																	**
**********************************************************************/
extern TS_AUTOMATA g_automata; /*Automata que usa la aplicacion*/
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
 * @brief  Funcion para inicializaci�n de los perif�ricos.
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

}
/**
 * @brief  Funcion para elegir el paso a seguir en cada momento.
 *
 * @return    -
 *
 * Primero se lee la tecla pulsada, luego se actualiza la pantalla teniendo
 * en cuenta la tecla pulsa, si se pulsa el select se env�a el mensaje y se
 * reinicializa la parte de la pantalla en la que escribe el usuario.
 * Si se ha reibido algo por la UART y est� almacenado en el buffer de software
 * se muestra en pantalla.
*/
void BROADCAR_logica(){
	MOTOR_AUTOMATA_ejecutar(&g_automata);
}
/**
 * @brief  Funcion que inicializa el array que contiene el valor de los sensores.
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
