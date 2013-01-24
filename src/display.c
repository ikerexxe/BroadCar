/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : display.c												**
** version : 1 														**
** date : 2012-10-08	 											**
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
**Version : 3														**
**Date : 2013-01-17													**
**Revised by : iker pedrosa											**
**Description : Modified version for broadcar application.			**
*********************************************************************/
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "rit128x96x4.h"
#include "display.h"
#include "displayGenerico.h"
#include "framebuffer.h"
#include "hw_types.h"
#include <stdlib.h>
#include <string.h>
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define DISPLAY_C
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern unsigned long g_ul_keypad_switches; /*Valor leído en los botones*/
extern unsigned char g_ucChangedData; /*Si ha cambiado la tecla que se está pulsando*/
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
static int g_i_numero_elemento = 0; /*Identificador del ultimo texto introducido en pantalla*/
static int g_i_altura_conversacion = 10; /*Altura en la cual se encuentra la conversacion*/

const unsigned char g_puc_circ[60]  =  {
        0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
        0x00, 0x4a, 0xaa, 0xaa, 0xa4, 0x00,
        0x04, 0xaa, 0xaa, 0xaa, 0xaa, 0x40,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x04, 0xaa, 0xaa, 0xaa, 0xaa, 0x40,
        0x00, 0x4a, 0xaa, 0xaa, 0xa4, 0x00,
        0x00, 0x00, 0x44, 0x44, 0x00, 0x00
    }; /*Dibujo de un círculo*/
const unsigned char g_puc_nada[60]  =  {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }; /*Dibujo vacío del tamaño del círculo*/
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
//TODO: no hay
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Inicializamos la pantalla.
 *
 * @return    -
 *
 * Primero se inicializa el buffer y luego se inicializa el texto del usuario.
 * Al final, se vuelca a la pantalla real.
*/
void BROADCAR_inicializacion_display(){
	unsigned char * str = malloc(sizeof(unsigned char) * 10); /*Cadena de caracteres que contiene inicia*/

	FRAME_BUFFER_init();
	strcpy(str, "inicia");
	FRAME_BUFFER_insert_text(str, 0, g_i_altura_conversacion);
	g_i_altura_conversacion += 10;
	g_i_numero_elemento++;
	FRAME_BUFFER_write_to_display();
}

/**
 * @brief  Insertar el último mensaje en la conversación
 *
 * @return    -
 *
 * Inserta el ultimo mensaje recibido mediante zigbee en
 * la conversación
*/
void BROADCAR_escribir(unsigned char * mensaje){
	int contador = 0; /*Contador para reubicar lo que se muestra en pantalla como si fuese una conversacion*/
	int altura = 10; /*Altua base de la conversacion*/

	if(g_i_numero_elemento < MAX_ELEMS_PANTALLA){
		g_i_numero_elemento = FRAME_BUFFER_insert_text(mensaje, 0, g_i_altura_conversacion);
		g_i_altura_conversacion += 10;
	} else{
		contador = 0;
		g_i_numero_elemento = FRAME_BUFFER_delete_element(contador);
		for(contador; contador < g_i_numero_elemento; contador++){
			FRAME_BUFFER_actualiza_posicion_elemento(contador, 0, altura);
			altura += 10;
		}
		g_i_numero_elemento = FRAME_BUFFER_insert_text(mensaje, 0, altura);
	}

	FRAME_BUFFER_write_to_display();
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
