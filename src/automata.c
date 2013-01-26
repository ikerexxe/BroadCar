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
#include <stdlib.h>
#include <stdio.h>
#include "hw_types.h"
#include "automata.h"
#include "motorAutomatas.h"
#include "keypad.h"
#include "display.h"
#include "sensorVisibilidad.h"
#include "sensorEstado.h"
#include "sensorObras.h"
#include "sensorVelocidad.h"
#include "zigbee.h"
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
tBoolean BROADCAR_TR_no_finalizar_estado(void);
tBoolean BROADCAR_TR_finalizar_estado(void);
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
	TRANSICION(11, BROADCAR_TR_cambio_visibilidad, NULL),
	TRANSICION(12, BROADCAR_TR_cambio_estado, NULL),
	TRANSICION(13, BROADCAR_TR_cambio_obras, NULL),
	TRANSICION(14, BROADCAR_TR_cambio_velocidad, NULL),
	TRANSICION(2, BROADCAR_TR_finalizar_estado, NULL)
};
FIN_ESTADO(Sensores, 1, BROADCAR_ACCION_sensores)

ESTADO(Mensajes){
	TRANSICION(1, BROADCAR_TR_finalizar_estado,NULL)
};
FIN_ESTADO(Mensajes, 2, BROADCAR_ACCION_mensajes)

/*
 * Estados secundarios, donde se mira el estado de los sensores de:
 * niebla, velocidad, hielo y obras
 */
ESTADO(Niebla){
	TRANSICION(2, BROADCAR_TR_finalizar_estado, NULL)
};
FIN_ESTADO(Niebla, 11, BROADCAR_ACCION_poca_visibilidad)

ESTADO(Hielo){
	TRANSICION(2, BROADCAR_TR_finalizar_estado,NULL)
};
FIN_ESTADO(Hielo, 12, BROADCAR_ACCION_estado_carretera)

ESTADO(Obras){
	TRANSICION(2, BROADCAR_TR_finalizar_estado,NULL)
};
FIN_ESTADO(Obras, 13, BROADCAR_ACCION_obras)

ESTADO(Velocidad_lenta){
	TRANSICION(2, BROADCAR_TR_finalizar_estado,NULL)
};
FIN_ESTADO(Velocidad_lenta, 14, BROADCAR_ACCION_velocidad)

/*
 * Máquina de estados
 */
AUTOMATA(automata){
	&Sensores,
	&Mensajes,
	&Niebla,
	&Hielo,
	&Obras,
	&Velocidad_lenta
};
FIN_AUTOMATA(automata, 101, BROADCAR_AUT_finish)
/*********************************************************************
**																	**
** LOCAL FUNCTIONS													**
** 																	**
**********************************************************************/
/**
 * @brief  Funcion que siempre permite el franqueo de la transicion
*/
tBoolean BROADCAR_TR_finalizar_estado(void){
	int i = 0;

	for(i = 0; i < 250000; i++){

	}

	return true;
}
/**
 * @brief  Funcion que no permite el franqueo de la transicion
*/
tBoolean BROADCAR_TR_no_finalizar_estado(void){
	int i = 0;

	for(i = 0; i < 250000; i++){

	}

	return false;
}

/**
 * @brief  Funcion que ejecuta el automata de los sensores
*/
void BROADCAR_ACCION_sensores(void){
	BROADCAR_leer_keypad();
	BROADCAR_escribir("sensor");
}
/**
 * @brief  Funcion que ejecuta el automata de los mensajes
*/
void BROADCAR_ACCION_mensajes(void){
	unsigned char * pantalla = malloc(sizeof(unsigned char) * 20);
	MENSAJEClass m_mensaje; /*Cuerpo del mensaje que se recibe por zigbee*/
	tBoolean b_mensaje = false; /*Si se ha recibido un mensaje completo*/

	b_mensaje = BROADCAST_hay_mensaje();
	if(b_mensaje){
		m_mensaje = BROADCAST_recibir_mensaje();
		if(m_mensaje.id != NULL){
			if(g_i_numero_mensaje > MAX_MENSAJES){
				borrar_mensaje();
			}
			g_mc_mensajes[g_i_numero_mensaje] = m_mensaje;
			g_i_numero_mensaje++;
			//TODO: enviar mendiante bluetooth
		}
		sprintf(pantalla, "mensaje %d", m_mensaje.id);
		BROADCAR_escribir(pantalla);
	}else{
		BROADCAR_escribir("no mensaje");
	}
}
/**
 * @brief  Funcion que decide si acabar el programa del automata
*/
tBoolean BROADCAR_AUT_finish(void){
	return false;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
