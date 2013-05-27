/*
 * uartDrvSpartan.c
 *
 *  Created on: 24/05/2013
 *      Author: Daniel
 */

#include "uartDrv.h"

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

XUartLite uart_UartLite;			/* The instance of the UART */

XIntc InterruptController;  /* The instance of the Interrupt Controller */

extern int led_data[6];
extern int cont_data;
extern int indice_uart;
unsigned char *a;
extern int uart_recibido;
volatile int uart_recv;
extern int contador_letra;

void UART_open(int nPort)
{
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
}

void interrupcion_uart_send(void *CallBackRef, unsigned int EventData)
{
	XUartLite *UartLitePtr = (XUartLite *)CallBackRef;
	// Recibir un byte de la UART
	//XUartLite_Recv(UartLitePtr, &led_data, 1);
	led_data[0]++;
	if(cont_data<contador_letra)
	{
		XUartLite_Send(&uart_UartLite, led_data, 1);
		cont_data++;
	}

	//button_pressed = 1;
}

void interrupcion_uart_recv(void *CallbackRef, unsigned int EventData)
{
	XUartLite *uart_UartLite = (XUartLite *)CallbackRef;
	/*int length;
	u8 buffer_ptr[5];*/

	//Receive the interrupt

	//buffer_uart = XUartLite_Recv(uart_UartLite,buffer_ptr,1);
	/*buffer_uart[indice_uart]*/*(a+indice_uart) = (u8)XUartLite_RecvByte(XPAR_USB_UART_BASEADDR);
	if(*(a+indice_uart)==0x20)
	{
		uart_recibido=1;
	}
	else
	{
		indice_uart++;
	}

    /*if (length != 0)*/
    	//Update the LEDs with the value recived
    	//led_data = buffer_ptr[0];

   	XUartLite_EnableInterrupt(uart_UartLite);
}

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

int UART_send(int nPort, unsigned char *p, int *pSize)
{
	contador_letra = (*pSize)-1;
	XUartLite_Send(&uart_UartLite, p, 1);
	return 1;
}

