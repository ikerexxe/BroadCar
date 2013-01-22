/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : automataPrincipal.h									**
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
#include "hw_types.h" //TODO: esto esta bien aqui?

/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
*********************************************************************/
#ifndef AUTOMATAPRINCIPAL_H
#define AUTOMATAPRINCIPAL_H

/*********************************************************************
** 																	**
** PROTOTYPES OF GLOBAL FUNCTIONS 									**
** 																	**
*********************************************************************/

void BROADCAR_ACCION_sensores(void);
void BROADCAR_ACCION_mensajes(void);
void BROADCAR_ACCION_poca_visibilidad(void);
void BROADCAR_ACCION_estado_carretera(void);
void BROADCAR_ACCION_obras(void);
void BROADCAR_ACCION_velocidad(void);
tBoolean BROADCAR_AUT_finish(void);

/*********************************************************************
**	 																**
** TYPEDEFS AND STRUCTURES 											**
** 																	**
*********************************************************************/
#ifndef AUTOMATAPRINCIPAL_C
#define PUB_AUTOMATAPRINCIPAL extern
#else
#define PUB_AUTOMATAPRINCIPAL
#endif
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
*********************************************************************/
#endif
