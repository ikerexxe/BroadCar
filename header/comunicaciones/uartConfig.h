/*****************************************************************************
**											  								**
** project :  ChatLM3S8962					 							  	**
** filename : uartConfig.h 						  					    	**
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
** date : 2013-01-22	 													**
**Revised by : iker pedrosa                    	  							**
**Description : Modified version for broadcar application.					**
*****************************************************************************/
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef UARTCONFIG_H
#define UARTCONFIG_H

#define BUFF_SIZE 256
#define BAUD_RATE 9600
#define DATA_FRAME (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
