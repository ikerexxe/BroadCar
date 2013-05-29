/*
 * displaySpartan.c
 *
 *  Created on: 27/05/2013
 *      Author: Daniel
 */

#include "string.h"
#include "stdio.h"
#include "displaySpartan.h"
#include "xparameters.h"
#include "xtmrctr_l.h"
#include "xtmrctr.h"
#include "xgpio.h"
#include "xgpio_l.h"

XTmrCtr DelayTimer1;

extern int indice_uart;
extern int longitud;
extern int linea,indice,uart_recibido,recibido;
extern int contador;
extern unsigned char *a;
extern char *cont_aux;
static unsigned char texto_aux[20];
extern int datos;
extern u8 buffer_uart[256];

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
