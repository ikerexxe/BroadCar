/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : displaySpartan.c										**
** version : 1 														**
** date : 2012-05-27	 											**
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
**Date : 2013-05-27													**
**Revised by : daniel collado										**
**Description : Original version.									**
*********************************************************************/
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "displaySpartan.h"
#include "xparameters.h"
#include "xtmrctr_l.h"
#include "xtmrctr.h"
#include "xgpio.h"
#include "xgpio_l.h"
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define DISPLAYSPARTAN_C
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
XTmrCtr DelayTimer1;
extern int indice_uart; //TODO: por que esta variable se encuentra aqui y se inicializa aqui?
int longitud;
int linea,indice,uart_recibido,recibido;
int contador; //TODO: no seria mas correcto ponerle otro nombre para saber lo que cuenta?
extern unsigned char *a; //TODO: por que esta variable se encuentra aqui si solo es global a uartDrvSpartan?
char *cont_aux; //TODO: no seria mas correcto ponerle otro nombre para saber lo que cuenta?
static unsigned char texto_aux[20];
int datos; //TODO: no seria mas correcto ponerle otro nombre para saber que son esos datos?
unsigned char buffer_uart[256]; //TODO: esta variable es propia de la uart? con ese nombre da a entender eso
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void elegir_linea(int line);
void escribir(int linea, unsigned char *text);
void pasar_a_string(int cont);
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
//TODO: a comentar lo que hace esta funcion
void DISPLAY_escribir(unsigned char * mensaje)
{
	int i;
	indice_uart=0;
	longitud=strlen(mensaje);
	linea++;
	if(linea<3)
	{
		//pasar_a_string(contador);
		//strcat(mensaje,cont_aux);
		//longitud=strlen(mensaje);
		escribir(linea, mensaje);
	}


	if(linea>2)
	{
		// esperar 1.52ms para RETURN HOME
		XTmrCtr_SetResetValue(&DelayTimer1, 1,50667);// esperar 1.52ms
		XTmrCtr_Start(&DelayTimer1, 1);
		while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
		{

		}
		XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x401); //return home

		XTmrCtr_SetResetValue(&DelayTimer1, 1,34);// esperar 1us
		XTmrCtr_Start(&DelayTimer1, 1);
		while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
		{

		}
		XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x1); //escribir con flanco de bajada del enable

		linea=2;
		//pasar_a_string(contador);
		//strcat(mensaje,cont_aux);
		longitud=strlen(texto_aux);
		escribir(1, texto_aux);
		longitud=strlen(mensaje);
		escribir(2, mensaje);
	}

	for(i=0;i<20;i++)
	{
		texto_aux[i]=0;
	}

	for(i=0;i<longitud;i++)
	{
		texto_aux[i]=*(mensaje+i);
	}
	//strcpy(texto_aux,mensaje);


	for(indice = 0 ; indice < longitud ; indice++)
	{
		*(mensaje+indice)=0;
		*(a+indice)=0;
		buffer_uart[indice]=0;
	}
	recibido=0;

uart_recibido=0;
}
//TODO: a comentar lo que hace esta funcion
void DISPLAY_inicializacion(void)
{
	XTmrCtr_Initialize(&DelayTimer1, 1);
	XTmrCtr_SetOptions(&DelayTimer1,1,XTC_DOWN_COUNT_OPTION);

	// esperar 20ms para SET FUNCTION
	XTmrCtr_SetResetValue(&DelayTimer1, 1,666667);
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x438);// set function

	XTmrCtr_SetResetValue(&DelayTimer1, 1,34);// esperar 1us
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x38); //escribir con flanco de bajada del enable

	// esperar 37us para DISPLAY SET
	XTmrCtr_SetResetValue(&DelayTimer1, 1,1234);// esperar 37us
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x40C); //display set

	XTmrCtr_SetResetValue(&DelayTimer1, 1,34);// esperar 1us
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0xC); //escribir con flanco de bajada del enable

	// esperar 37us para DISPLAY CLEAR
	XTmrCtr_SetResetValue(&DelayTimer1, 1,1234);// esperar 37us
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x401); //display clear

	XTmrCtr_SetResetValue(&DelayTimer1, 1,34);// esperar 1us
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x1); //escribir con flanco de bajada del enable

	// esperar 1.52ms para RETURN HOME
	XTmrCtr_SetResetValue(&DelayTimer1, 1,50667);// esperar 1.52ms
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x401); //return home

	XTmrCtr_SetResetValue(&DelayTimer1, 1,34);// esperar 1us
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x1); //escribir con flanco de bajada del enable
}
//TODO: a comentar lo que hace esta funcion y ademas a�adirle el DISPLAY por delante del nombre de la funcion
void elegir_linea(int line)
{
	int inicio_linea;
	if(line==1)
	{
		inicio_linea=0x80;
	}
	else
	{
		inicio_linea=0xC0;
	}
	// esperar 1.52ms para RETURN HOME
	XTmrCtr_SetResetValue(&DelayTimer1, 1,50667);// esperar 1.52ms
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x400|inicio_linea); //return home

	XTmrCtr_SetResetValue(&DelayTimer1, 1,34);// esperar 1us
	XTmrCtr_Start(&DelayTimer1, 1);
	while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
	{

	}
	XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, inicio_linea); //escribir con flanco de bajada del enable
}
//TODO: a comentar lo que hace esta funcion y ademas a�adirle el DISPLAY por delante del nombre de la funcion
void pasar_a_string(int cont)
{
	int temporal=10,index=0,orden=1;
	int digito = 0;
	int cant_digitos=0;
	temporal=cont;

	while(temporal>0)
	{
		temporal /= 10;
		cant_digitos++;
	}

	cant_digitos--;
	orden=cant_digitos;
	temporal=cont;
	digito = temporal % 10;
	*(cont_aux + cant_digitos) = digito+0x30;

	cant_digitos--;

	temporal /= 10;
	digito = temporal % 10;
	*(cont_aux + cant_digitos) = digito+0x30;

	cant_digitos--;

	temporal /= 10;
	digito = temporal % 10;
	*(cont_aux + cant_digitos) = digito+0x30;

	cant_digitos--;

	temporal /= 10;
	digito = temporal % 10;
	*(cont_aux + cant_digitos) = digito+0x30;

	orden=1;
}
//TODO: a comentar lo que hace esta funcion y ademas a�adirle el DISPLAY por delante del nombre de la funcion
void escribir(int linea, unsigned char *text)
{
	if(linea==1)
	{
		elegir_linea(1);
	}
	else
	{
		elegir_linea(2);
	}
	for(indice = 0 ; indice < longitud ; indice++)
	{
		datos=*(text+indice);
		// esperar 120us para escribir un 1
		XTmrCtr_SetResetValue(&DelayTimer1, 1,50667);// esperar 120us
		XTmrCtr_Start(&DelayTimer1, 1);
		while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
		{

		}
		XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x500|datos); //return home

		XTmrCtr_SetResetValue(&DelayTimer1, 1,34);// esperar 1us
		XTmrCtr_Start(&DelayTimer1, 1);
		while(!XTmrCtr_IsExpired(&DelayTimer1, 1))
		{

		}
		XGpio_WriteReg(XPAR_PANTALLA_BASEADDR, 0, 0x100|datos); //escribir con flanco de bajada del enable
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
