/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : uartDrvSpartan.c										**
** version : 1 														**
** date : 2012-05-24	 											**
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
**Version : 1														**
**Date : 2013-05-24													**
**Revised by : daniel collado										**
**Description : Original version.									**
*********************************************************************/
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "uartDrvSpartan.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"
#include "stdio.h"
#include "xstatus.h"
#include "stdlib.h"
#include "string.h"
#include "xbasic_types.h"
#include "xintc_l.h"       //LibrerÃ­a del Controlador de interrupciones
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define UARTDRVSPARTAN_C
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
XUartLite uart_UartLite;			/* The instance of the UART */
XIntc InterruptController;  /* The instance of the Interrupt Controller */
//TODO: todas estas variables hay que hacerlas globales a este fichero solo
extern int led_data[256];
extern int cont_data;
extern int indice_uart;
extern unsigned char *a;
extern int uart_recibido;
volatile int uart_recv;
extern int contador_letra;
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void interrupcion_uart_send(void *CallBackRef, unsigned int EventData);
void interrupcion_uart_recv(void *CallBackRef, unsigned int EventData);
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
//TODO: a comentar lo que hace esta funcion
void UART_open(int nPort)
{
	XIntc_Initialize(&InterruptController, XPAR_MICROBLAZE_0_INTC_DEVICE_ID);
	// Inicializar UART
	XUartLite_Initialize(&uart_UartLite, XPAR_UARTLITE_2_DEVICE_ID);

	// Conectar el Interrupt Handler al controlador de interrupciones
	XIntc_Connect(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID,(XInterruptHandler)XUartLite_InterruptHandler, (void *)&uart_UartLite);

	// Habilitamos la interrupciï¿½n 2 del controlador de interrupciones
	XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID);

	// Asignar la funciï¿½n de interrupciï¿½n de recepciï¿½n
	XUartLite_SetRecvHandler(&uart_UartLite, interrupcion_uart_recv, &uart_UartLite);
	// Asignar la funciï¿½n de interrupciï¿½n de envï¿½o
	XUartLite_SetSendHandler(&uart_UartLite, interrupcion_uart_send, &uart_UartLite);
	// Habilitar interrupciones en la UART
	XUartLite_EnableInterrupt(&uart_UartLite);

	// Inicializar UART
//	XUartLite_Initialize(&uart_UartLite, XPAR_UARTLITE_2_DEVICE_ID);

	// Asignar la funciï¿½n de interrupciï¿½n de recepciï¿½n
//	XUartLite_SetRecvHandler(&uart_UartLite, interrupcion_uart_recv, &uart_UartLite);
	// Asignar la funciï¿½n de interrupciï¿½n de envï¿½o
//	XUartLite_SetSendHandler(&uart_UartLite, interrupcion_uart_send, &uart_UartLite);
	// Habilitar interrupciones en la UART
//	XUartLite_EnableInterrupt(&uart_UartLite);
	// InicializaciÃ³n del controlador de interrupciones
//	XIntc_Initialize(&InterruptController, XPAR_MICROBLAZE_0_INTC_DEVICE_ID);
	// Conectar el Interrupt Handler al controlador de interrupciones
//	XIntc_Connect(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID,(XInterruptHandler)XUartLite_InterruptHandler, (void *)&uart_UartLite);

	// Habilitamos la interrupciï¿½n 2 del controlador de interrupciones
//	XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID);
}
//TODO: a comentar lo que hace esta funcion y ademas añadirle el DISPLAY por delante del nombre de la funcion
void interrupcion_uart_send(void *CallBackRef, unsigned int EventData)
{
	XUartLite *UartLitePtr = (XUartLite *)CallBackRef;
	// Recibir un byte de la UART
	//XUartLite_Recv(UartLitePtr, &led_data, 1);
	//led_data[0]++;
	if(cont_data<contador_letra)
	{
		XUartLite_Send(&uart_UartLite, led_data+cont_data, 1);
		cont_data++;
	}
	else
	{
		cont_data=1;
	}

	//button_pressed = 1;
}
//TODO: a comentar lo que hace esta funcion y ademas añadirle el DISPLAY por delante del nombre de la funcion
void interrupcion_uart_recv(void *CallbackRef, unsigned int EventData)
{
	XUartLite *uart_UartLite = (XUartLite *)CallbackRef;
	/*int length;
	u8 buffer_ptr[5];*/

	//Receive the interrupt

	//buffer_uart = XUartLite_Recv(uart_UartLite,buffer_ptr,1);
	/*buffer_uart[indice_uart]*/
	*(a+indice_uart) = (u8)XUartLite_RecvByte(XPAR_USB_UART_BASEADDR);
	/*if(*(a+indice_uart)==0x20)
	{
		uart_recibido=1;
	}
	else
	{
		indice_uart++;
	}*/

    /*if (length != 0)*/
    	//Update the LEDs with the value recived
    	//led_data = buffer_ptr[0];

   	XUartLite_EnableInterrupt(uart_UartLite);
}
//TODO: a comentar lo que hace esta funcion
int UART_recv(int nPort, unsigned char *p, int *pSize)
{
	int n;
	int i;
	n = *pSize;
	for(i=0;i<n;i++)
	{
		p[i]=*(a+i);
	}
	return 1;
}
//TODO: a comentar lo que hace esta funcion
int UART_send(int nPort, unsigned char *p, int *pSize)
{
	int i;
	contador_letra = (*pSize)-1;
	for(i=0;i<contador_letra;i++)
	{
		led_data[i]=p[i];
	}
	XUartLite_Send(&uart_UartLite, p, 1);
	return 1;
}
//TODO: a comentar lo que hace esta funcion e implementarla
void UART_close(int nPort){

}
//TODO: a comentar lo que hace esta funcion e implementarla
int UART_nElementosIn(int nPort){

}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
