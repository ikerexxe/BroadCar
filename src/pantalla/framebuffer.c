/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : framebuffer.c											**
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
**Version : 2														**
**Date : 2012-12-05													**
**Revised by : iker pedrosa											**
**Description : Modified version for the chat.						**
*********************************************************************/
#define CONSOLE_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include <stdlib.h>
#include "framebuffer.h"
#include "rit128x96x4.h"
#include "displayGenerico.h"
#include "broadcar.h"
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define CHAR_WIDTH 6
#define LINE_HEIGHT 8
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
static DISPLAY_ELEMENT gs_display_elements[MAX_ELEMS_PANTALLA];
static int gs_display_element_kop = 0; /*Num. de elementos que hay en el buffer*/
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern const unsigned char g_puc_nada[60];/*Dibujo vacío del tamaño del círculo*/
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Inicialización de la pantalla y el buffer.
 *
 * @return        -
 *
 * Se inicializa la pantalla y se crea el buffer para un número de elementos definido en MAX_ELEMS
*/
void FRAME_BUFFER_init(){
	int i = 0;
	//Inicializamos la pantalla
	DISPLAY_GENERICO_init();
	//Creamos el buffer
	for (i=0; i<MAX_ELEMS_PANTALLA; i++){
		gs_display_elements[i].id = i;
		gs_display_elements[i].x = i;
		gs_display_elements[i].y = i;
		gs_display_elements[i].width = i;
		gs_display_elements[i].height = i;
		gs_display_elements[i].picture_or_text = i;
		gs_display_elements[i].what = i;
	}
}
/**
 * @brief  Insertamos una imagen en el buffer
 *
 * @return  display_element_kop: numero de elementos que hay en el buffer
 *
 * Se crea un elemento del tipo what = 0 (imagen) y se especifica la imagen que debe tomar,
 * la posición en la que debe aparecer y las dimensiones que tiene. Se devuelve el numero de
 * elementos que hay en el buffer despues de haber añadido ésta imagen.
*/
int FRAME_BUFFER_insert_image(char *puc, int x, int y, int w, int h){
	if(gs_display_element_kop < MAX_ELEMS_PANTALLA){
		gs_display_elements[gs_display_element_kop].id = gs_display_element_kop;
		gs_display_elements[gs_display_element_kop].picture_or_text = puc;
		gs_display_elements[gs_display_element_kop].what = 0; //0 para imagen
		gs_display_elements[gs_display_element_kop].x = x;
		gs_display_elements[gs_display_element_kop].y = y;
		gs_display_elements[gs_display_element_kop].width = w;
		gs_display_elements[gs_display_element_kop].height = h;
		gs_display_element_kop ++; //Aumentamos el número de elementos que hay en el buffer
	}
	return gs_display_element_kop;
}
/**
 * @brief  Insertamos texto en el buffer
 *
 * @return  display_element_kop: numero de elementos que hay en el buffer
 *
 * Se crea un elemento del tipo what = 1 (texto) y se especifica el texto que debe tomar,
 * la posición en la que debe aparecer. Las dimensiones serán nulas. Se devuelve el numero de
 * elementos que hay en el buffer despues de haber añadido éste texto.
*/
int FRAME_BUFFER_insert_text(char *texto, int x, int y){
	if(gs_display_element_kop < MAX_ELEMS_PANTALLA){
		gs_display_elements[gs_display_element_kop].id = gs_display_element_kop;
		gs_display_elements[gs_display_element_kop].picture_or_text = texto;
		gs_display_elements[gs_display_element_kop].what = 1; //1 para texto
		gs_display_elements[gs_display_element_kop].x = x;
		gs_display_elements[gs_display_element_kop].y = y;
		gs_display_elements[gs_display_element_kop].width = 0;
		gs_display_elements[gs_display_element_kop].height = 0;
		gs_display_element_kop ++; //Aumentamos numero de elementos que hay en el buffer
	}
	return gs_display_element_kop;
}
/**
 * @brief  Cambiamos la posición de un elemento existente
 *
 * @return  -
 *
 * Se guardan las coordenadas anteriores en xOld e yOld, y se guardan las nuevas
 * coordenadas en su sitio.
*/
void FRAME_BUFFER_actualiza_posicion_elemento(int id, int x, int y){
	int x_actual; /*Posicion actual en el eje x*/
	int y_actual; /*Posicion actual en el eje y*/
	//Obtener posición actual
	x_actual = gs_display_elements[id].x;
	y_actual = gs_display_elements[id].y;
	//Borrar del FrameBuffer
	gs_display_elements[id].xOld = x_actual;
	gs_display_elements[id].yOld = y_actual;
	//Actualizar valor
	gs_display_elements[id].x = x;
	gs_display_elements[id].y = y;
}
/**
 * @brief  Cambiamos el texto de un elemento existente.
 *
 * @return  -
 *
 * Se actualiza el texto del elemento.
*/
void FRAME_BUFFER_actualiza_texto_elemento(int id, char *texto){
	gs_display_elements[id].picture_or_text = texto;
}
/**
 * @brief  Borramos un elemento del buffer
 *
 * @return  display_element_kop: numero de elementos que hay en el buffer
 *
 * Sacamos el elemento de la pantalla, y desplazamos los elementos con una id mayor
 * a la posición anterior del buffer. Reduce una vez el numero de elementos del
 * buffer y lo devuelve.
*/
int FRAME_BUFFER_delete_element(int id){
	int x_actual; /*Posición del elemento en el eje x*/
	int y_actual; /*Posición del elemento en el eje y*/
	int i;
	//Obtener posición actual
	x_actual = gs_display_elements[id].x;
	y_actual = gs_display_elements[id].y;
	//Borrar del FrameBuffer
	gs_display_elements[id].xOld = x_actual;
	gs_display_elements[id].yOld = y_actual;
	//Actualizar valor
	gs_display_elements[id].x = -5;
	gs_display_elements[id].y = -5;
	//Desasignar memoria
	free(gs_display_elements[id].picture_or_text);

	for(i=id;i<MAX_ELEMS_PANTALLA;i++){
		gs_display_elements[i].picture_or_text = gs_display_elements[i + 1].picture_or_text;
		gs_display_elements[i].what = gs_display_elements[i + 1].what; //1 para texto
		gs_display_elements[i].x = gs_display_elements[i + 1].x;
		gs_display_elements[i].y = gs_display_elements[i + 1].y;
		gs_display_elements[i].width = gs_display_elements[i + 1].width;
		gs_display_elements[i].height = gs_display_elements[i + 1].height;

	}
	//Reducimos numero de elementos que hay en el buffer
	gs_display_element_kop =  gs_display_element_kop - 1;
	return gs_display_element_kop;
}
/**
 * @brief  Volcar el buffer en la pantalla
 *
 * @return  -
 *
 * Se vuelcan todos los elementos existentes en el bufefr en la pantalla.
 * Se mira uno a uno si el elemento es un texto o un dibujo, y se usan las
 * funciones para cada uno de los casos para borrar lo innecesario y para
 * mostrar los elementos necesarios.
*/
void FRAME_BUFFER_write_to_display(void){
	int i=0;
	int size = 0; /*Tamaño del string*/
	int index =0; /*Indice de posición en el array str*/
	char str[30]; /*Array donde se va guardar el string*/
	for(i=0;i < gs_display_element_kop; i++){ //Volcamos todos los elementos existentes en en buffer
		if(( gs_display_elements[i].xOld != gs_display_elements[i].x ) ||
		(gs_display_elements[i].yOld != gs_display_elements[i].y ))
		{
			if(gs_display_elements[i].what==1){ //Es un texto
					size = MAX_ELEMS_LINEA;
					for(index = 0; index < size; index ++){
						str[index]= ' ';
					}
					str[index]='\0';
					if(i==0 && gs_display_element_kop == MAX_ELEMS_PANTALLA){
						DISPLAY_GENERICO_dibuja_string(str, gs_display_elements[i].x,gs_display_elements[i].y, 15);
					}
					DISPLAY_GENERICO_dibuja_string(str, gs_display_elements[i].xOld,gs_display_elements[i].yOld, 15);
					DISPLAY_GENERICO_dibuja_string(gs_display_elements[i].picture_or_text, gs_display_elements[i].x,gs_display_elements[i].y, 15);
				}else //Es una imagen
				{
					DISPLAY_GENERICO_dibuja_imagen(g_puc_nada, gs_display_elements[i].xOld, gs_display_elements[i].yOld, gs_display_elements[i].width, gs_display_elements[i].height);
					DISPLAY_GENERICO_dibuja_imagen(gs_display_elements[i].picture_or_text, gs_display_elements[i].x, gs_display_elements[i].y, gs_display_elements[i].width, gs_display_elements[i].height);
				}
		}
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/




