/*****************************************************************************
**											  								**
** project :  BroadCar						 							  	**
** filename : sensorEstado.h					  					    	**
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
#ifndef SENSORESTADO_H
#define SENSORESTADO_H
/*****************************************************************************
** 																			**
** PROTOTYPES OF GLOBAL FUNCTIONS											**
** 																			**
******************************************************************************/
tBoolean BROADCAR_TR_cambio_estado(void);
void BROADCAR_ACCION_estado_carretera(void);
/*****************************************************************************
**	 																		**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
#ifndef SENSORESTADO_C
#define PUB_SENSORESTADO extern
#else
#define PUB_SENSORESTADO
#endif
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
