/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : automataPrincipal.c									**
** version : 1 														**
** date : 2013-01-21	 											**
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
**Date : 2013-01-21													**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "hw_types.h"
#include "automata.h"
#include "motorAutomatas.h"
#include "keypad.h"
#include "display.h"
#include "zigbee.h"
#include "sensorVisibilidad.h"
#include "sensorEstado.h"
#include "sensorObras.h"
#include "sensorVelocidad.h"
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
tBoolean cambio_no_finalizar_estado(void);
tBoolean cambio_finalizar_estado(void);
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define AUTOMATAPRINCIPAL_C
/*
 * Estados principales del automata
 */
ESTADO(Sensores){
	TRANSICION(11, S_VISIBILIDAD_cambio, NULL),
	TRANSICION(12, S_ESTADO_cambio, NULL),
	TRANSICION(13, S_OBRAS_cambio, NULL),
	TRANSICION(14, S_VELOCIDAD_cambio, NULL),
	TRANSICION(2, cambio_finalizar_estado, NULL)
};
FIN_ESTADO(Sensores, 1, AUTOMATA_sensores)

ESTADO(Mensajes){
	TRANSICION(1, cambio_finalizar_estado,NULL)
};
FIN_ESTADO(Mensajes, 2, ZIGBEE_recepcion_mensajes)

/*
 * Estados secundarios, donde se mira el estado de los sensores de:
 * niebla, velocidad, hielo y obras
 */
ESTADO(Niebla){
	TRANSICION(2, cambio_finalizar_estado, NULL)
};
FIN_ESTADO(Niebla, 11, S_VISIBILIDAD_accion)

ESTADO(Hielo){
	TRANSICION(2, cambio_finalizar_estado,NULL)
};
FIN_ESTADO(Hielo, 12, S_ESTADO_accion)

ESTADO(Obras){
	TRANSICION(2, cambio_finalizar_estado,NULL)
};
FIN_ESTADO(Obras, 13, S_OBRAS_accion)

ESTADO(Velocidad_lenta){
	TRANSICION(2, cambio_finalizar_estado,NULL)
};
FIN_ESTADO(Velocidad_lenta, 14, S_VELOCIDAD_accion)

/*
 * Máquina de estados
 */
AUTOMATA(g_automata){
	&Sensores,
	&Mensajes,
	&Niebla,
	&Hielo,
	&Obras,
	&Velocidad_lenta
};
FIN_AUTOMATA(g_automata, 101, AUTOMATA_finish)
/*********************************************************************
**																	**
** LOCAL FUNCTIONS													**
** 																	**
**********************************************************************/
/**
 * @brief  Funcion que siempre permite el franqueo de la transicion
*/
tBoolean cambio_finalizar_estado(void){
	int i = 0;

	for(i = 0; i < 5000; i++){

	}

	return true;
}
/**
 * @brief  Funcion que no permite el franqueo de la transicion
*/
tBoolean cambio_no_finalizar_estado(void){
	int i = 0;

	for(i = 0; i < 5000; i++){

	}

	return false;
}

/**
 * @brief  Funcion que ejecuta el automata de los sensores
*/
void AUTOMATA_sensores(void){
	KEYPAD_leer();
}
/**
 * @brief  Funcion que decide si acabar el programa del automata
*/
tBoolean AUTOMATA_finish(void){
	return false;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
