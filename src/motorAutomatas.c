/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : motorAutomatas.c										**
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
**Version : 2														**
**Date : 2013-01-21													**
**Revised by : iker pedrosa											**
**Description : Modified version for broadcar application.			**
*********************************************************************/

/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "display.h"
#include "motorAutomatas.h"

/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define MOTORAUTOMATAS_C

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
BYTE fms_mv = 1;
BYTE fms_mv_anterior;
int get_id_motor = 0;

/*********************************************************************
**																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/

/**
 * @brief  Funcion que ejecuta el automata
 *
*/
void EjecutaAutomata(TS_AUTOMATA *elAutomata)
{
	unsigned char error[10] = "ERROR!!!!";
	TS_ESTADO **Indx;    /* Valor indice rastreador */

	//Si ha llegado a la condicion final, termina de ejecutar el automata.
	if (elAutomata->StopCond())  return;

	//Ira pasando por cada uno de los estados hasta que no haya mas
	for (Indx = elAutomata->estado; *Indx != NULL; ++Indx)
	{
	    if (fms_mv == (*Indx)->id)
	    {
	    	//En cada estado ejecutara las funciones que le corresponda.
			EjecutaEstado(*Indx);
			return;
	    }
	}
	//Si falla mostrará un error.
	BROADCAR_escribir(error);
}

/**
 * @brief  Funcion que ejecuta el estado
 *
*/
void EjecutaEstado(TS_ESTADO *elEstado)
{
	TS_EVEACC *Indx;   /* Indice de rastreo */

	(*elEstado->controles)();
	//recorre las funciones dentro del estado hasta que no haya mas.
	for (Indx = elEstado->funcion; Indx->evento != NULL; ++Indx)
	{
		//Ejecuta esto si existe una transicion para acceder al siguiente evento
		if (Indx->evento() == true)
		{
			//Ejecuta esto si existe una accion dentro de este evento.
			if (Indx->accion != NULL)
			{
				//Ejecuta la accion correspondiente dentro del evento.
				Indx->accion();
			}
			if (Indx->id != fms_mv)
			{
				fms_mv_anterior = fms_mv;
				fms_mv = Indx->id;

				return;
			}
		}
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
