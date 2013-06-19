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
#include "broadcar.h"
#ifdef SPARTAN
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
#include "xgpio.h"
#include "xgpio_l.h"
int num_mensaje;
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
XUartLite uart_UartLite_1;			/* The instance of the UART */
XIntc InterruptController;  /* The instance of the Interrupt Controller */
volatile int uart_recv;
int buffer_enviar[256];
int buffer_bluetooth_enviar[256];
int cont_data=1;
int cont_data_bluetooth=1;
int indice_buffer_entrada=0;
int indice_buffer_bluetooth_entrada=0;
unsigned char buffer_recibir[256];
unsigned char buffer_bluetooth_recibir[256];
int uart_recibido;
int contador_letra;
int contador_letra_bluetooth;
int contador_recibir;
int contador_recibir_bluetooth;
int enable_lectura_fifo=0;
int datos_fifo[256];
int fifo_vacia;
int fifo_llena;
int reloj;
int flag=0;
int i=0;
int indice_entrada=0;
int longitud_trama=33;
int longitud_trama_bluetooth=20;
unsigned char letra=0x30;
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void interrupcion_uart_send(void *CallBackRef, unsigned int EventData);
void interrupcion_uart_recv(void *CallBackRef, unsigned int EventData);

void interrupcion_uart_send_bluetooth(void *CallBackRef, unsigned int EventData);
void interrupcion_uart_recv_bluetooth(void *CallBackRef, unsigned int EventData);
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Funcion para inicializar uno de los puertos UART.
 *
 * @return    -
 *
 * Inicializa el puerto de la UART.
*/
void UART_open(int nPort)
{
	num_mensaje = 0;

	XIntc_Initialize(&InterruptController, XPAR_MICROBLAZE_0_INTC_DEVICE_ID);
	// Inicializar UART
	XUartLite_Initialize(&uart_UartLite, XPAR_UARTLITE_2_DEVICE_ID);
	XUartLite_Initialize(&uart_UartLite_1, XPAR_UARTLITE_0_DEVICE_ID);

	// Conectar el Interrupt Handler al controlador de interrupciones
	XIntc_Connect(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID,(XInterruptHandler)XUartLite_InterruptHandler, (void *)&uart_UartLite);
	XIntc_Connect(&InterruptController, XPAR_INTC_0_UARTLITE_0_VEC_ID,(XInterruptHandler)XUartLite_InterruptHandler, (void *)&uart_UartLite_1);

	// Habilitamos la interrupci�n 2 del controlador de interrupciones
	XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID);
	XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_0_VEC_ID);

	// Asignar la funci�n de interrupci�n de recepci�n
	XUartLite_SetRecvHandler(&uart_UartLite, interrupcion_uart_recv, &uart_UartLite);
	XUartLite_SetRecvHandler(&uart_UartLite_1, interrupcion_uart_recv_bluetooth, &uart_UartLite_1);
	// Asignar la funci�n de interrupci�n de env�o
	XUartLite_SetSendHandler(&uart_UartLite, interrupcion_uart_send, &uart_UartLite);
	XUartLite_SetSendHandler(&uart_UartLite_1, interrupcion_uart_send_bluetooth, &uart_UartLite_1);
	// Habilitar interrupciones en la UART
	XUartLite_EnableInterrupt(&uart_UartLite);
	XUartLite_EnableInterrupt(&uart_UartLite_1);
}
/**
 * @brief  Rutina de interrupcion.
 *
 * @return    -
 *
 * Rutina de interrupcion del envio de la UART.
*/
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
/**
 * @brief  Rutina de interrupcion.
 *
 * @return    -
 *
 * Rutina de interrupcion de la recepcion de la UART.
*/
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
	if((indice_buffer_entrada==33)){
		indice_buffer_entrada=33;
		num_mensaje++;
	}
	 /*if (num_mensaje==11)
	 {
		 num_mensaje = num_mensaje+1;
	 }*/
	 if (num_mensaje==12)
	 {
		 num_mensaje = num_mensaje;
	 }



    /*if (length != 0)*/
    	//Update the LEDs with the value recived
    	//led_data = buffer_ptr[0];

   	XUartLite_EnableInterrupt(uart_UartLite);
}

void interrupcion_uart_recv_bluetooth(void *CallbackRef, unsigned int EventData)
{
	XUartLite *uart_UartLite_1 = (XUartLite *)CallbackRef;

	//Receive the interrupt
	*(buffer_bluetooth_recibir+indice_buffer_bluetooth_entrada)=(u8)XUartLite_RecvByte(XPAR_AXI_UARTLITE_0_BASEADDR);
	//*(buffer_recibir+indice_buffer_entrada) = (u8)XUartLite_RecvByte(XPAR_USB_UART_BASEADDR);
	indice_buffer_bluetooth_entrada++;

   	XUartLite_EnableInterrupt(uart_UartLite_1);
}

void interrupcion_uart_send_bluetooth(void *CallBackRef, unsigned int EventData)
{
	//XUartLite *UartLitePtr = (XUartLite *)CallBackRef;
	// Recibir un byte de la UART
	//XUartLite_Recv(UartLitePtr, &led_data, 1);
	//led_data[0]++;
	if(cont_data_bluetooth < contador_letra_bluetooth)
	{
		XUartLite_Send(&uart_UartLite_1, buffer_bluetooth_enviar+cont_data_bluetooth, 1);
		cont_data_bluetooth++;
	}
	else
	{
		cont_data_bluetooth=1;
	}

	//button_pressed = 1;
}

int UART_recibir(void)
{
	int indice;

	int bytes_recibidos=0;
	int reloj;
	unsigned char buffer_intermedio[16];


	//XGpio_WriteReg(XPAR_LEDS_4BITS_BASEADDR, 0, fifo_vacia*2);

	/*for(i=0;i<1000000;i++)
	{
		XGpio_WriteReg(XPAR_LEDS_4BITS_BASEADDR, 0, fifo_vacia*2);
	}*/
	//if(XGpio_ReadReg(XPAR_DIP_SWITCHES_4BITS_BASEADDR,0))
		//{
			//datos_fifo[i] = XGpio_ReadReg(XPAR_SERIE_IN_BASEADDR,0);
			//if(reloj==1 && fifo_vacia==0 && flag==0)
			//{
				//if(enable_lectura_fifo==1)
				//{


				/*for(indice=0;indice<100;indice++)
				{
					buffer_recibir[i] = XGpio_ReadReg(XPAR_SERIE_IN_BASEADDR,0);
				}*/
				/*if(!XGpio_ReadReg(XPAR_FIFO_VACIA_BASEADDR,0))
				{
					XGpio_WriteReg(XPAR_READ_ENABLE_BASEADDR, 0, 1);
				}
				else
				{
					XGpio_WriteReg(XPAR_READ_ENABLE_BASEADDR, 0, 0);
				}

				if(!XGpio_ReadReg(XPAR_RELOJ_FIFO_IN_BASEADDR,0))//No hay dato disponible
				{
					flag=1;
				}
				else
				{
					if(flag)
					{
						buffer_recibir[i] = XGpio_ReadReg(XPAR_SERIE_IN_BASEADDR,0);
						i++;
						flag=0;
					}
				}*/


					//buffer_recibir[i] = XGpio_ReadReg(XPAR_SERIE_IN_BASEADDR,0);
				/*for(indice=0;indice<100;indice++)
				{
					buffer_recibir[i] = XGpio_ReadReg(XPAR_SERIE_IN_BASEADDR,0);
				}*/
					/*XGpio_WriteReg(XPAR_READ_ENABLE_BASEADDR, 0, 0);

					if(XGpio_ReadReg(XPAR_FIFO_VACIA_BASEADDR,0))
					{
						i=i;
					}
					i++;*/


					//XGpio_WriteReg(XPAR_READ_ENABLE_BASEADDR, 0, 1);//Leer fifo
					//enable_lectura_fifo=0;
					//flag=1;
				/*}
				else
				{
					XGpio_WriteReg(XPAR_READ_ENABLE_BASEADDR, 0, 0);//Leer fifo
					flag=1;
					enable_lectura_fifo=1;
				}*/
			/*}
			else
			{
				if(reloj==0)
				{
					flag=0;
				}
			}*/



	bytes_recibidos = XUartLite_Recv(&uart_UartLite,buffer_intermedio,16);
	for(indice=indice_entrada;indice<(indice_entrada+bytes_recibidos);indice++)
	{
		buffer_recibir[indice]=buffer_intermedio[i];
		i++;
	}
	indice_entrada+=bytes_recibidos;
	if(indice_entrada>=18)
	{
		indice_entrada=18;
	}
	return 1;
}

/**
 * @brief  Guarda los datos en un buffer de software.
 *
 * @return    -
 *
 * Guarda los datos en un buffer de software para usarse en la logica de la aplicacion.
*/
int UART_recv(int nPort, unsigned char *p, int *pSize)
{
	int n;
	int indice;

	if(nPort==1)
	{
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
	}
	else
	{
		if(contador_recibir_bluetooth < 1){
			n = *pSize;
			for(;contador_recibir_bluetooth<n;contador_recibir_bluetooth++)
			{
				p[contador_recibir_bluetooth]=*(buffer_bluetooth_recibir+contador_recibir_bluetooth);
			}
		}else{
			n = (*pSize) + contador_recibir_bluetooth;
			for(;contador_recibir_bluetooth<n;contador_recibir_bluetooth++)
			{
				p[contador_recibir_bluetooth-1]=*(buffer_bluetooth_recibir+contador_recibir_bluetooth);
			}
			for(contador_recibir_bluetooth=0;contador_recibir_bluetooth<n;contador_recibir_bluetooth++)
			{
				*(buffer_bluetooth_recibir+contador_recibir_bluetooth)=0;
			}
			contador_recibir_bluetooth = 0;
		}
		indice_buffer_bluetooth_entrada=0;
	}
	return 1;
}
/**
 * @brief  Guarda los datos temporalmente en un buffer para enviarse.
 *
 * @return    -
 *
 * Guarda los datos a enviar en un buffer para posteriormente enviarlos.
*/
int UART_send(int nPort, unsigned char *p, int *pSize)
{
	int i;

	if(nPort==1)
	{
		contador_letra = (*pSize);
		for(i=1;i<contador_letra;i++)
		{
			buffer_enviar[i]=p[i];
		}
		XUartLite_Send(&uart_UartLite, p, 1);
	}
	else
	{
		contador_letra_bluetooth = (*pSize);
		for(i=1;i<contador_letra_bluetooth;i++)
		{
			buffer_bluetooth_enviar[i]=p[i];
		}
		XUartLite_Send(&uart_UartLite_1, p, 1);
	}
	return 1;
}
/**
 * @brief  Cerrar puerto UART.
 *
 * @return    -
 *
 * Cierra un puerto UART.
*/
void UART_close(int nPort){

}
/**
 * @brief  Calcula longitud del mensaje.
 *
 * @return    -
 *
 * Calcula la longitud del mensaje que esta llegando y verifica cuando llega completo.
*/
int UART_nElementosIn(int nPort){
	int indice;

	if(nPort==1)
	{
		indice_entrada=indice_buffer_entrada;
		if(indice_entrada>=3)
		{
			if(buffer_recibir[2]==0x1d)
			{
				longitud_trama=33;
			}

			if(buffer_recibir[2]==0x1e)
			{
				longitud_trama=34;
			}
		}
	}
	else
	{
		indice_entrada=indice_buffer_bluetooth_entrada;
		if(buffer_bluetooth_recibir[0]=='N')
		{
			longitud_trama_bluetooth=20;
		}

		if(buffer_bluetooth_recibir[0]=='R')
		{
			longitud_trama_bluetooth=33;
		}

		if(buffer_bluetooth_recibir[0]=='S')
		{
			longitud_trama_bluetooth=38;
		}
	}
	/*if(indice_buffer_entrada>=longitud_trama)
	{
		indice_buffer_entrada=0;
	}*/
	return indice_entrada;
}
#endif
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
