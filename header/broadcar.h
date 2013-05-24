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
#include "data.h"
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef BROADCAR_H
#define BROADCAR_H
/*
 * Definición de la arquitectura
 */
#define STELLARIS
//#define SPARTAN
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
#define MAX_ELEMS_LINEA 16
/*
 * Numero de sensores de los que dispone el vehiculo
 */
#define NUMERO_SENSORES 4
/*
 * Numero de mensajes que puede guardar el vehiculo
 */
#define MAX_MENSAJES 16
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
SENSORClass g_cs_sensores[NUMERO_SENSORES]; /*Valores de todos los sensores del vehiculo*/
int g_i_hora; /*Hora del sistema*/
int g_i_numero_mensaje; /*Indice dle numero de mensajes que se han recibido*/
int g_i_mi_id; /*Identificador del vehiculo*/
unsigned long g_ul_keypad_switches; /*Valor leído en los botones*/
unsigned char g_uc_changed_data; /*Si ha cambiado la tecla que se está pulsando*/
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
