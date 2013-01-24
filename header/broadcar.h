/*****************************************************************************
**											  								**
** project :  BroadCar						 							  	**
** filename : chat.h	  						  					    	**
** version : 1 									  							**
** date :  	2013-01-17		 		 									    **
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
**Date : 2013-01-17									 					    **
**Revised by : iker pedrosa                    	  							**
**Description : Modified version for the chat.				  				**
*****************************************************************************/
/*****************************************************************************
**																			**
** MODULES USED 															**
** 																			**
*****************************************************************************/
#include "data.h" //TODO: esto esta bien aqui?
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef BROADCAR_H
#define BROADCAR_H
/*
 * Definición de las teclas
 */
#define KEY_UP 0x1e     //11110
#define KEY_DOWN 0x1d   //11101
#define KEY_LEFT 0x1b   //11011
#define KEY_RIGHT 0x17  //10111
#define KEY_SELECT 15 //01111
/*
 * Definición del numero maximo de caracteres por linea
 */
#define MAX_ELEMS_LINEA 10
/*
 * Numero de sensores de los que dispone el coche
 */
#define NUMERO_SENSORES 4
/*
 * Posicion del array asignada a cada sensor
 */
#define SENSOR_VISIBILIDAD 0
#define SENSOR_ESTADO 1
#define SENSOR_OBRAS 2
#define SENSOR_VELOCIDAD 3
/*
 * Definición de NULL
 */
#ifndef NULL
#define NULL 0
#endif
/*****************************************************************************
** 																			**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
/*
 * Definición de BYTE
 */
typedef char BYTE;
/*****************************************************************************
** 																			**
** GLOBAL VARIABLES 														**
** 																			**
*****************************************************************************/
SENSORClass g_sc_sensores[NUMERO_SENSORES]; /*Valores de todos los sensores del vehiculo*/
int g_i_hora; /*Hora del sistema*/
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
