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
