/*****************************************************************************
**											  								**
** project :  BroadCar						 							  	**
** filename : sensorObras.h						  					    	**
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
#ifndef SENSOROBRAS_H
#define SENSOROBRAS_H
/*****************************************************************************
**																			**
** MODULES USED 															**
** 																			**
******************************************************************************/
#include "broadcar.h"
/*****************************************************************************
** 																			**
** PROTOTYPES OF GLOBAL FUNCTIONS											**
** 																			**
******************************************************************************/
boolean S_OBRAS_cambio(void);
void S_OBRAS_accion(void);
/*****************************************************************************
**	 																		**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
#ifndef SENSOROBRAS_C
#define PUB_SENSOROBRAS extern
#else
#define PUB_SENSOROBRAS
#endif
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
