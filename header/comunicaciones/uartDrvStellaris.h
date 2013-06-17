/*****************************************************************************
**											  								**
** project :  ChatLM3S8962					 							  	**
** filename : uartDrvStellaris.h				  					    	**
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
**Version : 1										  						**
**Date : 2012-12-16									 					    **
**Revised by : iker pedrosa                    	  							**
**Description : Modified version for the chat.				  				**
*****************************************************************************/
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef UARTDRVSTELLARIS_H
#define UARTDRVSTELLARIS_H
/*****************************************************************************
** 																			**
** PROTOTYPES OF GLOBAL FUNCTIONS											**
** 																			**
******************************************************************************/
int UART_open(int nPort);
void UART_close(int nPort);
int UART_recv(int nPort, unsigned char *p, int *pSize);
int UART_send(int nPort, unsigned char *p, int *pSize);
int UART_nElementosIn(int nPort);
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif