/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : automataPrincipal.h									**
** version : 1 														**
** date : 2012-10-31	 											**
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
**Description : Modified version for broadcar application.			**
*********************************************************************/
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "broadcar.h" //TODO: esto esta bien aqui?
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
*********************************************************************/
#ifndef MOTORAUTOMATAS_H
#define MOTORAUTOMATAS_H

#define ESTADO(nombre_estado) const TS_EVEACC x##nombre_estado[] =
#define TRANSICION(id,evento,accion) {id, #evento, evento, accion}
#define FIN_ESTADO(nombre_fin_estado,id,control) const TS_ESTADO nombre_fin_estado = {id,#id,control,(TS_EVEACC *) x##nombre_fin_estado};
#define AUTOMATA(nombre_automata) const TS_ESTADO * const (x##nombre_automata[]) =
#define FIN_AUTOMATA(nombre_automata,id,StopC) const TS_AUTOMATA nombre_automata = {id, #nombre_automata, StopC, (TS_ESTADO **) x##nombre_automata};
/*********************************************************************
**	 																**
** TYPEDEFS AND STRUCTURES 											**
** 																	**
*********************************************************************/
//Definicion del tipo Accion y Evento
typedef void (*TPNF_ACC)(void);
typedef boolean (*TPNF_EVE)(void);

//Definicion del tipo de dato que representa una transicion
typedef struct{
	BYTE 			id; /*Identificador del estado destino */
	char 		  * nombre; /*Nombre identificativo de depuración*/
	TPNF_EVE 		evento; /*Puntero a la función evento*/
	TPNF_ACC 		accion; /*Puntero a la función evento*/
} TS_EVEACC;

//Definición del tipo Estado
typedef struct {
	  BYTE          id;             /* Identificador del estado */
	  char        * nombre;         /* Nombre identificativo de depuraci¢n */
	  TPNF_EVE      controles;      /* Funci¢n de control del estado */
	  TS_EVEACC   * funcion;        /* Punteros a las funciones del estado */
} TS_ESTADO;

//Definición del tipo Maquina de estado
typedef struct {
	  BYTE          id;             /* Identificador del automata */
	  char        * nombre;         /* Nombre identificativo de depuraci¢n */
	  TPNF_EVE      StopCond;       /* Condici¢n necesaria para la ejecuci¢n */
	  TS_ESTADO   **estado;         /* Matriz de punteros a los ESTADOS */
} TS_AUTOMATA;

#ifndef MOTORAUTOMATAS_H
#define PUB_MOTORAUTOMATAS extern
#else
#define PUB_MOTORAUTOMATAS
#endif
/*********************************************************************
** 																	**
** PROTOTYPES OF GLOBAL FUNCTIONS									**
** 																	**
*********************************************************************/
void MOTOR_AUTOMATA_ejecutar( TS_AUTOMATA *elAutomata);
void MOTOR_AUTOMATA_ejecuta_estado( TS_ESTADO   *elEstado);
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
*********************************************************************/
#endif
