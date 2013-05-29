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
#include "xintc_l.h"       //Librería del Controlador de interrupciones
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
volatile int uart_recv;
int buffer_enviar[256];
int cont_data=1;
int indice_buffer_entrada;
unsigned char buffer_recibir[256];
int uart_recibido;
int contador_letra;
int contador_recibir;
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

	// Habilitamos la interrupci�n 2 del controlador de interrupciones
	XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID);

	// Asignar la funci�n de interrupci�n de recepci�n
	XUartLite_SetRecvHandler(&uart_UartLite, interrupcion_uart_recv, &uart_UartLite);
	// Asignar la funci�n de interrupci�n de env�o
	XUartLite_SetSendHandler(&uart_UartLite, interrupcion_uart_send, &uart_UartLite);
	// Habilitar interrupciones en la UART
	XUartLite_EnableInterrupt(&uart_UartLite);

	// Inicializar UART
//	XUartLite_Initialize(&uart_UartLite, XPAR_UARTLITE_2_DEVICE_ID);

	// Asignar la funci�n de interrupci�n de recepci�n
//	XUartLite_SetRecvHandler(&uart_UartLite, interrupcion_uart_recv, &uart_UartLite);
	// Asignar la funci�n de interrupci�n de env�o
//	XUartLite_SetSendHandler(&uart_UartLite, interrupcion_uart_send, &uart_UartLite);
	// Habilitar interrupciones en la UART
//	XUartLite_EnableInterrupt(&uart_UartLite);
	// Inicialización del controlador de interrupciones
//	XIntc_Initialize(&InterruptController, XPAR_MICROBLAZE_0_INTC_DEVICE_ID);
	// Conectar el Interrupt Handler al controlador de interrupciones
//	XIntc_Connect(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID,(XInterruptHandler)XUartLite_InterruptHandler, (void *)&uart_UartLite);

	// Habilitamos la interrupci�n 2 del controlador de interrupciones
//	XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID);
}
//TODO: a comentar lo que hace esta funcion y ademas a�adirle el DISPLAY por delante del nombre de la funcion
void interrupcion_uart_send(void *CallBackRef, unsigned int EventData)
{
	XUartLite *UartLitePtr = (XUartLite *)CallBackRef;
	// Recibir un byte de la UART
	//XUartLite_Recv(UartLitePtr, &led_data, 1);
	//led_data[0]++;
	if(cont_data<contador_letra)
	{
		XUartLite_Send(&uart_UartLite, buffer_enviar+cont_data, 1);
		cont_data++;
	}
	else
	{
		cont_data=1;
	}

	//button_pressed = 1;
}
//TODO: a comentar lo que hace esta funcion y ademas a�adirle el DISPLAY por delante del nombre de la funcion
void interrupcion_uart_recv(void *CallbackRef, unsigned int EventData)
{
	XUartLite *uart_UartLite = (XUartLite *)CallbackRef;
	/*int length;
	u8 buffer_ptr[5];*/

	//Receive the interrupt

	//buffer_uart = XUartLite_Recv(uart_UartLite,buffer_ptr,1);
	/*buffer_uart[indice_uart]*/
	*(buffer_recibir+indice_buffer_entrada) = (u8)XUartLite_RecvByte(XPAR_USB_UART_BASEADDR);
	indice_buffer_entrada++;
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

	if(contador_recibir < 3){
		n = *pSize;
		for(;contador_recibir<n;contador_recibir++)
		{
			p[contador_recibir]=*(buffer_recibir+contador_recibir);
		}
	}else{
		n = (*pSize) + contador_recibir;
		for(;contador_recibir<n;contador_recibir++)
		{
			p[contador_recibir-3]=*(buffer_recibir+contador_recibir);
		}
		for(contador_recibir=0;contador_recibir<n;contador_recibir++)
		{
			*(buffer_recibir+contador_recibir)=0;
		}
		contador_recibir = 0;
	}
	indice_buffer_entrada=0;
	return 1;
}
//TODO: a comentar lo que hace esta funcion
int UART_send(int nPort, unsigned char *p, int *pSize)
{
	int i;
	contador_letra = (*pSize);
	for(i=1;i<contador_letra;i++)
	{
		buffer_enviar[i]=p[i];
	}
	XUartLite_Send(&uart_UartLite, p, 1);
	return 1;
}
//TODO: a comentar lo que hace esta funcion e implementarla
void UART_close(int nPort){

}
//TODO: a comentar lo que hace esta funcion e implementarla
int UART_nElementosIn(int nPort){
	return indice_buffer_entrada-1;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
