/*****************************************************************************
**											  								**
** project :  BroadCar						 							  	**
** filename : sensorVisibilidad.h				  					    	**
** version : 1 									  							**
** date :  	2013-01-22		 		 									    **
** 											  								**
******************************************************************************
** 											  								**
**                      											  		**
**  								  					                    **
** 											  								**
******************************************************************************
**										        							**
**VERSION HISTORY:									  						**
**----------------									  						**
**Version : 1										  						**
**Date : 2013-01-22									 					    **
**Revised by : iker pedrosa                    	  							**
**Description : Original version.							  				**
*****************************************************************************/
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef SENSORVISIBILIDAD_H
#define SENSORVISIBILIDAD_H

/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "broadcar.h"
/*****************************************************************************
** 																			**
** PROTOTYPES OF GLOBAL FUNCTIONS											**
** 																			**
******************************************************************************/
tBoolean S_VISIBILIDAD_cambio(void);
void S_VISIBILIDAD_accion(void);
/*****************************************************************************
**	 																		**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
#ifndef SENSORVISIBILIDAD_C
#define PUB_SENSORVISIBILIDAD extern
#else
#define PUB_SENSORVISIBILIDAD
#endif
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
