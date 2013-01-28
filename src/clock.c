/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : clock.c 												**
** version : 1 														**
** date : 2012-10-08	 											**
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
**Date : 2012-12-05													**
**Revised by : iker pedrosa											**
**Description : Modified version for the chat.						**
*********************************************************************/
#define CLOCK_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "hw_types.h"
#include "systick.h"
#include "interrupt.h"
#include "clock.h"
#include "driverlib/sysctl.h"
#include "broadcar.h"
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
*********************************************************************/
static unsigned long g_ul_system_clock; /*Frecuencia del clock*/
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Función para inicializar el clock.
 *
 * @return    -
 *
 *
*/
void BROADCAR_inicializacion_clock(){
	//125ns
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |SYSCTL_XTAL_8MHZ);
	SysTickPeriodSet(8000000);//Configuramos las interrupciones a 1 segundo.
	SysTickIntEnable();//habilitamos la interrupcion del Systick del sistema.
	SysTickEnable();//habilitamos el conteo de los tick del sistema.
	IntMasterEnable();//Habilitamos las interrupciones.
	g_ul_system_clock = SysCtlClockGet();
	g_i_hora = 0;
}
/**
 * @brief  Funcion que crea una interrupcion cada segundo
*/
void __attribute__((interrupt)) IntUnSegundo(void){
	g_i_hora++;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/

