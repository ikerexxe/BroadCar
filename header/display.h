/*****************************************************************************
**											  								**
** project :  ChatLM3S8962					 							  	**
** filename : display.h  						  					    	**
** version : 1 									  							**
** date :  	2012-10-08		 		 									    **
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
**Version : 2										  						**
**Date : 2012-12-05									 					    **
**Revised by : iker pedrosa                    	  							**
**Description : Modified version for broadcar application.					**
*****************************************************************************/
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H
/*****************************************************************************
** 																			**
** PROTOTYPES OF GLOBAL FUNCTIONS											**
** 																			**
******************************************************************************/
void DISPLAY_inicializacion();
void DISPLAY_escribir(unsigned char * mensaje);
/*****************************************************************************
**	 																		**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
#ifndef DISPLAY_C
#define PUB_DISPLAY extern
#else
#define PUB_DISPLAY
#endif
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
