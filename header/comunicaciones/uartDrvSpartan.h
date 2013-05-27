/*
 * uartDrv.h
 *
 *  Created on: 24/05/2013
 *      Author: Daniel
 */

#ifndef UARTDRV_H_
#define UARTDRV_H_


void interrupcion_uart_send(void *CallBackRef, unsigned int EventData);
void interrupcion_uart_recv(void *CallBackRef, unsigned int EventData);


/*#include "xintc.h"
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"
#include "stdio.h"
#include "xstatus.h"*/
void UART_open(int nPort);

int UART_recv(int nPort, unsigned char *p, int *pSize);
int UART_send(int nPort, unsigned char *p, int *pSize);

#endif /* UARTDRV_H_ */
