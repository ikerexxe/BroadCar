/*********************************************************************
 ** 																	**
 ** project : ChatLM3S8962	 										**
 ** filename : uartDrv.c 											**
 ** version : 1 														**
 ** date : 2012-12-05	 											**
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
 ** date : 2013-01-21	 											**
 **Revised by : iker pedrosa											**
 **Description : Original version.									**
 *********************************************************************/
#define UARTDRV_C
/*********************************************************************
 **																	**
 ** MODULES USED 													**
 ** 																	**
 **********************************************************************/
#include "uartDrv.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "uartConfig.h"
#include <stdlib.h>
/*****************************************************************************
 **	 																		**
 ** TYPEDEFS AND STRUCTURES 													**
 ** 																			**
 *****************************************************************************/
typedef struct{
	//for reception
	int inHead, inTail;
	unsigned char inBuf[BUFF_SIZE];
	//for transmission
	int outHead, outTail;
	int  writingToBuf;
	int intWhileWriting;
	unsigned char outBuf[BUFF_SIZE];
}UARTClass;
/*********************************************************************
 ** 																	**
 ** GLOBAL VARIABLES 												**
 ** 																	**
 **********************************************************************/
static unsigned long gs_ul_uarts_bases[]={UART0_BASE,UART1_BASE}; /*Identificadores de las UART*/
static unsigned long gs_ul_uarts_ints[]={INT_UART0,INT_UART1}; /*Interrupciones de las UART*/
/*Creamos los objetos de comunicaciones: UARTClass*/
static UARTClass gs_cu_uarts[3]; /*Clase que contiene los datos de la UART*/
/*********************************************************************
 ** 																	**
 ** PROTOTYPES OF LOCAL FUNCTIONS 									**
 ** 																	**
 *********************************************************************/
static int nHuecosIn(int nPort);
static int nElementosOut(int nPort);
static int nHuecosOut(int nPort);
void UART0_IntHandler(void);
void UART1_IntHandler(void);
static void fromHwFIFO(int nPort);
static int toHwFIFO(int nPort);
static void initUartPins(int nPort);
void UARTIntHandlerLogic(int nPort);
void  __attribute__((interrupt)) UART0_IntHandler(void);
void  __attribute__((interrupt)) UART1_IntHandler(void);
/*********************************************************************
 **																	**
 ** LOCAL FUNCTIONS													**
 ** 																	**
 **********************************************************************/
/**
 * @brief  Función para inicializar los PINS de la UART.
 *
 * @return    -
 *
 * Inicializa los PINs para poder usar la UART.
 */
static void initUartPins(int nPort)
{
	switch(nPort)
	{
	case 0: SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
			GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
			break;
	case 1: SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
			break;
	default:
			break;
	}
}
/**
 * @brief  Función para inicializar uno de los puertos UART.
 *
 * @return    -
 *
 * Inicializa el puerto 0 de la UART.
 */
int UART_open(int nPort)
{
	initUartPins(nPort);
	UARTDisable(gs_ul_uarts_bases[nPort]);
	switch(nPort)
	{
		case 0: UARTConfigSetExpClk(gs_ul_uarts_bases[nPort], SysCtlClockGet(), BAUD_RATE_0, DATA_FRAME);
				break;
		case 1: UARTConfigSetExpClk(gs_ul_uarts_bases[nPort], SysCtlClockGet(), BAUD_RATE_1, DATA_FRAME);
				break;
		default:
				break;
	}
	UARTEnable(gs_ul_uarts_bases[nPort]);
	gs_cu_uarts[nPort].inHead = 0;
	gs_cu_uarts[nPort].inTail = 0;
	gs_cu_uarts[nPort].inHead = 0;
	gs_cu_uarts[nPort].inTail = 0;
	gs_cu_uarts[nPort].intWhileWriting = 0;
	gs_cu_uarts[nPort].writingToBuf=0;
	UARTFIFOLevelSet(gs_ul_uarts_bases[nPort], UART_FIFO_TX1_8, UART_FIFO_RX1_8);
	IntEnable(gs_ul_uarts_ints[nPort]);
	UARTIntEnable(gs_ul_uarts_bases[nPort], UART_INT_TX | UART_INT_RX | UART_INT_RT);
	return 0;
}
/**
 * @brief  Función para cerrar uno de los puertos UART.
 *
 * @return    -
 *
 * Cierra el puerto 0 de la UART.
 */
void UART_close(int nPort)
{
	UARTDisable(gs_ul_uarts_bases[nPort]);
	IntDisable(INT_UART0);
}
/**
 * @brief  Función para retorna los datos recibidos por la UART.
 *
 * @return    Exito de la recepcion
 *
 * Retorna los datos que se han almacenado en el buffer de software a la
 * logica del usuario.
 */
int UART_recv(int nPort, unsigned char *p, int *pSize)
{
	int i; /*Contador que recorre el buffer de software*/
	int j; /*Contador que recorre el buffer de hardware*/
	int n; /*Numero de elementos en el buffer de hardware*/

	n=UART_nElementosIn(nPort);
	if((*pSize) != 0){
		n = *pSize;
	}
	*pSize=n;
	j=gs_cu_uarts[nPort].inTail;
	for(i=0;i<n;i++)
	{
		p[i]=gs_cu_uarts[nPort].inBuf[j++];
		if(j==BUFF_SIZE) j=0;
	}
	gs_cu_uarts[nPort].inTail=j;
	return 1;
}
/**
 * @brief  Función para enviar datos por la UART.
 *
 * @return    Existo del envio
 *
 * Guarda los datos a enviar en el el buffer de software.
 */
int UART_send(int nPort, unsigned char *p, int *pSize)
{
	int i; /*Contador que recorre el buffer de software*/
	int j; /*Contador que recorre el buffer de hardware*/
	int n; /*Numero de huecos en el buffer de hardware*/
	int retry; /*Si se debe seguir rellenando el buffer de hardware*/

	n=nHuecosOut(nPort);
	if(n>*pSize) n=*pSize;
	*pSize=n;
	j=gs_cu_uarts[nPort].outHead;
	for(i=0;i<n;i++)
	{
		gs_cu_uarts[nPort].outBuf[j++]=p[i];
		if(j==BUFF_SIZE) j=0;
	}
	gs_cu_uarts[nPort].outHead=j;
	do
	{
		gs_cu_uarts[nPort].writingToBuf=1;
		n=toHwFIFO(nPort);
		gs_cu_uarts[nPort].writingToBuf=0;
		if(gs_cu_uarts[nPort].intWhileWriting)
		{
			gs_cu_uarts[nPort].intWhileWriting=0;
			retry=gs_cu_uarts[nPort].outHead!=gs_cu_uarts[nPort].outTail;
		}
		else retry=0;
	}while(retry);
	return 1;
}
/**
 * @brief  Guarda los datos en el buffer del software.
 *
 * @return    -
 *
 * Guardado de los datos del buffer de hardware en el buffer de
 * software para su posterior tratamiento por la logica del usuario.
 */
static void fromHwFIFO(int nPort)
{
	int n; /*Numero de huecos en el buffer de hardware*/
	unsigned int l; /*Variable temporal donde se guardan los caracteres recibidos*/

	n=nHuecosIn(nPort);
	while(UARTCharsAvail(gs_ul_uarts_bases[nPort])&& (n-->0))
	{
		l=UARTCharGetNonBlocking(gs_ul_uarts_bases[nPort]);
		gs_cu_uarts[nPort].inBuf[gs_cu_uarts[nPort].inHead]=(unsigned char)l;
		gs_cu_uarts[nPort].inHead++;
		if(gs_cu_uarts[nPort].inHead==BUFF_SIZE) gs_cu_uarts[nPort].inHead=0;
	}
}
/**
 * @brief  Guarda los datos en el buffer del hardware.
 *
 * @return    -
 *
 * Guardado de los datos del buffer de software en el buffer de
 * hardware para su posterior envio.
 */
static int toHwFIFO(int nPort)
{
	unsigned char c; /*Variable temporal donde se guardan los caracteres a enviar*/
	int n; /*Numero de huecos en el buffer de hardware*/
	int m; /*Valor de retorno*/

	n=nElementosOut(nPort);
	m=0;
	while((n-->0)&&(UARTSpaceAvail(gs_ul_uarts_bases[nPort])))
	{
		c=gs_cu_uarts[nPort].outBuf[gs_cu_uarts[nPort].outTail];
		gs_cu_uarts[nPort].outTail++;
		if(gs_cu_uarts[nPort].outTail==BUFF_SIZE) gs_cu_uarts[nPort].outTail=0;
		UARTCharPutNonBlocking(gs_ul_uarts_bases[nPort], c);
	}
	return m;
}
/**
 * @brief  Rutina de interrupcion.
 *
 * @return    -
 *
 * Rutina de interrupcion de la UART 0.
 */
void  __attribute__((interrupt)) UART0_IntHandler(void)
{
	UARTIntHandlerLogic(0);
}
/**
 * @brief  Rutina de interrupcion.
 *
 * @return    -
 *
 * Rutina de interrupcion de la UART 1.
 */
void  __attribute__((interrupt)) UART1_IntHandler(void)
{
	UARTIntHandlerLogic(1);
}
/**
 * @brief  Tratamiento de interrupciones.
 *
 * @return    -
 *
 * Tratamiento de las interrupciones de la UART.
 */
void UARTIntHandlerLogic(int nPort)
{
	unsigned long ulStatus; /*El estado de la interrupcion*/

	ulStatus = UARTIntStatus(gs_ul_uarts_bases[nPort], true);
	if(ulStatus & (UART_INT_RX | UART_INT_RT ))
	{
		fromHwFIFO(nPort);
	}
	if(ulStatus & UART_INT_TX)
	{
		if(gs_cu_uarts[nPort].outHead!=gs_cu_uarts[nPort].outTail)
		{
			if(!gs_cu_uarts[nPort].writingToBuf) toHwFIFO(nPort);
			else gs_cu_uarts[nPort].intWhileWriting=1;
		}
		UARTIntClear(gs_ul_uarts_bases[nPort], UART_INT_TX);
	}
	UARTIntClear(gs_ul_uarts_bases[nPort], ulStatus);
}
/**
 * @brief  Numero de huecos libres en el buffer del hardware de entrada.
 *
 * @return    Numero de elementos
 *
 */
static int nHuecosIn(int nPort)
{
	return (gs_cu_uarts[nPort].inTail+BUFF_SIZE-gs_cu_uarts[nPort].inHead-1) % BUFF_SIZE;
}
/**
 * @brief  Numero de elementos en el buffer del hardware de entrada.
 *
 * @return    Numero de elementos
 *
 */
int UART_nElementosIn(int nPort)
{
	return (gs_cu_uarts[nPort].inHead+BUFF_SIZE-gs_cu_uarts[nPort].inTail) % BUFF_SIZE;
}
/**
 * @brief  Numero de huecos libres en el buffer del hardware de salida.
 *
 * @return    Numero de huecos libres
 *
 */
static int nHuecosOut(int nPort)
{
	return (gs_cu_uarts[nPort].outTail+BUFF_SIZE-gs_cu_uarts[nPort].outHead-1) % BUFF_SIZE;
}
/**
 * @brief  Numero de elementos en el buffer del hardware de salida.
 *
 * @return    Numero de elementos
 *
 */
static int nElementosOut(int nPort)
{
	return (gs_cu_uarts[nPort].outHead+BUFF_SIZE-gs_cu_uarts[nPort].outTail) % BUFF_SIZE;
}
/*********************************************************************
 ** 																	**
 ** EOF 																**
 ** 																	**
 **********************************************************************/
