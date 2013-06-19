/*********************************************************************
** 																	**
** project : BroadCar		 										**
** filename : bluetooth.c 											**
** version : 1 														**
** date : 2013-01-22	 											**
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
** date : 2013-05-22	 											**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/
#define ZIGBEE_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bluetooth.h"
#include "broadcar.h"
#ifdef STELLARIS
#include "hw_types.h"
#include "uartDrvStellaris.h"
#include "displayGenerico.h"
#include "displayStellaris.h"
#else
#include "displaySpartan.h"
#include "uartDrvSpartan.h"
#endif
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
boolean BLUETOOTH_hay_mensaje(void);
void BLUETOOTH_recibir_mensaje(void);
boolean BLUETOOTH_es_mensaje_emparejamiento();
boolean BLUETOOTH_es_mensaje_conexion();
boolean BLUETOOTH_es_mensaje_desconexion();
void BLUETOOTH_formatear_mensaje(MENSAJEClass mensaje);
void BLUETOOTH_calcular_llevada(int dato, int *contador_trama);
void BLUETOOTH_insertar_info_envio(MENSAJEClass mensaje, int *contador_trama);
void BLUETOOTH_enviar_emparejamiento(void);
void BLUETOOTH_vaciar_buffer_entrada(void);
void BLUETOOTH_vaciar_buffer_salida(void);
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern int longitud_trama_bluetooth; /*Longitud del mensaje que se recibe por bluetooth*/
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
boolean g_b_conectado = false;
static int gs_i_puerto_bluetooth = 0; /*Puerto UART que se usa para la comunicacion con el modulo bluetooth*/
static int gs_i_tamano = 0; /*Tama�o del mensaje*/
static uint8_t gs_ba_envio[255]; /*Mensaje a enviar en formato byte*/
static uint8_t gs_ba_recibido[255]; /*Mensaje recibido en formato byte*/
static unsigned char gs_uc_caracter_barra = 0x2F;
static unsigned char gs_uc_caracter_punto = 0x2E;
static unsigned char gs_i_cero_ascii = 48; /*Caracter cero en ASCII*/
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Funci�n para inicializar el puerto UART.
 *
 * @return    -
 *
 * Inicializa el puerto UART para poder comunicarse con el modulo
 * bluetooth
*/
void BLUETOOTH_inicializacion(){
	UART_open(gs_i_puerto_bluetooth);
}
/**
 * @brief  Funcion que ejecuta el automata de los mensajes
 *
 * Tipos de mensaje:
 * 		-Emparejamiento (12+18+8=38)
 * 			SSP CONFIRM e8:92:a4:45:b4:02 382028 ?
 * 			SSP CONFIRM e8:92:a4:45:b4:02 OK
 * 		-Conexion (7+18+8=33)
 *			RING 0 e8:92:a4:45:b4:02 1 RFCOMM
 *		-Desconexion (13+7=20)
 *			NO CARRIER 0 ERROR 0
*/
void BLUETOOTH_recepcion_mensajes(void){
	boolean b_mensaje = false; /*Si se ha recibido un mensaje completo*/
	unsigned char * pantalla; /*Donde se guarda lo que se va a escribir en pantalla*/

	b_mensaje = BLUETOOTH_hay_mensaje();
	if(b_mensaje){
		if(g_b_conectado){
			BLUETOOTH_recibir_mensaje();
			if(BLUETOOTH_es_mensaje_desconexion()){
				g_b_conectado = false;
				BLUETOOTH_vaciar_buffer_entrada();
				pantalla = malloc(sizeof(unsigned char) * 20);
				sprintf(pantalla, "DESCONECTADO");
				DISPLAY_escribir(pantalla);
			}
		}else{
			BLUETOOTH_recibir_mensaje();
			if(BLUETOOTH_es_mensaje_emparejamiento()){
				BLUETOOTH_enviar_emparejamiento();
				BLUETOOTH_vaciar_buffer_entrada();
				pantalla = malloc(sizeof(unsigned char) * 20);
				sprintf(pantalla, "EMPAREJADO");
				DISPLAY_escribir(pantalla);
			}else if(BLUETOOTH_es_mensaje_conexion()){
				g_b_conectado = true;
				BLUETOOTH_vaciar_buffer_entrada();
				pantalla = malloc(sizeof(unsigned char) * 20);
				sprintf(pantalla, "CONECTADO");
				DISPLAY_escribir(pantalla);
			}
		}
	}
}
/**
 * @brief  Funci�n para enviar una alerta mediante bluetooth.
 *
 * @return    -
 *
 * Vacia el buffer de salida, formatea el mensaje a enviar y
 * lo env�a mediante bluetooth
*/
void BLUETOOTH_enviar_mensaje(MENSAJEClass mensaje){
	BLUETOOTH_vaciar_buffer_salida();
	BLUETOOTH_formatear_mensaje(mensaje);
	UART_send(gs_i_puerto_bluetooth, gs_ba_envio, &gs_i_tamano);
}
/**
 * @brief  Funcion que verifica si es un mensaje de emparejamiento.
 *
 * @return    -
 *
 * Se verifica que el mensaje recibido por bluetooth es de
 * emparejamiento: SSP CONFIRM **:**:**:**:**:** ****** ?
 * Donde los asteriscos significan datos que cambian cada
 * vez que se envia el mensaje
*/
boolean BLUETOOTH_es_mensaje_emparejamiento(){
	boolean resultado = false;

	if(!strncmp(gs_ba_recibido, "SSP CONFIRM", 11)){
		resultado = true;
	}

	return resultado;
}
/**
 * @brief  Funcion que verifica si es un mensaje de conexion.
 *
 * @return    -
 *
 * Se verifica que el mensaje recibido por bluetooth es de
 * conexion: RING 0 **:**:**:**:**:** 1 RFCOMM
 * Donde los asteriscos significan datos que cambian cada
 * vez que se envia el mensaje
*/
boolean BLUETOOTH_es_mensaje_conexion(){
	boolean resultado = false;

	if(!strncmp(gs_ba_recibido, "RING", 4)){
		resultado = true;
	}

	return resultado;
}
/**
 * @brief  Funcion que verifica si es un mensaje de desconexion.
 *
 * @return    -
 *
 * Se verifica que el mensaje recibido por bluetooth es de
 * desconexion: NO CARRIER 0 ERROR 0
*/
boolean BLUETOOTH_es_mensaje_desconexion(){
	boolean resultado = false;

	if(!strncmp(gs_ba_recibido, "NO CARRIER 0 ERROR 0", 20)){
		resultado = true;
	}

	return resultado;
}
/**
 * @brief  Funci�n que mira si se ha recibido un mensaje por bluetooth.
 *
 * @return    Si se ha recibido un mensaje.
 *
 * Mira si se han recibido m�s de 33 bytes mediante bluetooth, indicativo
 * de que se ha recibido el mensaje completo.
*/
boolean BLUETOOTH_hay_mensaje(void){
	int numero_elementos = 0; /*Numero de elementos que hay en el buffer de software*/
	boolean completo = false; /*Si se ha recibido un mensaje completo*/

	numero_elementos = UART_nElementosIn(gs_i_puerto_bluetooth);
	if(g_b_conectado){
		if(numero_elementos >= longitud_trama_bluetooth/*18*/){
			completo = true;
		}
	}else{
		if(numero_elementos >= longitud_trama_bluetooth/*33*/){
			completo = true;
		}
	}

	return completo;
}
/**
 * @brief  Funci�n que recibe el mensaje de zigbee.
 *
 * @return    El cuerpo del mensaje.
 *
 * Hace el tratamiento de la trama zigbee para quedarse con el
 * cuerpo del mensaje y reenviar la trama entera al resto de
 * controladores que se encuentren a su alcance.
*/
void BLUETOOTH_recibir_mensaje(void){
	uint8_t temporal[255]; /*Guarda el mensaje temporalmente*/
	int numero_recibido = 1; /*Contador para trasladar los datos de temporal a recibido*/
	int contador = 0; /*Contador para trasladar los datos de temporal a recibido*/

	for(contador = 0; contador < 255; contador++){
		temporal[contador] = 0;
	}
	contador = 0;
	UART_recv(gs_i_puerto_bluetooth, temporal, &numero_recibido);

	while(!(temporal[0] == 'S' || temporal[0] == 'R' || temporal[0] == 'N')){
		UART_recv(gs_i_puerto_bluetooth, temporal, &numero_recibido);
	}

	gs_ba_recibido[0] = temporal[0];

	if(temporal[0] == 'S'){
		numero_recibido = 37;
	}else if(temporal[0] == 'R'){
		numero_recibido = 32;
	}else if(temporal[0] == 'N'){
		numero_recibido = 19;
	}

	UART_recv(gs_i_puerto_bluetooth, temporal, &numero_recibido);
	for(contador = 0; contador < numero_recibido; contador++){
		gs_ba_recibido[contador+1] = temporal[contador];
	}
}
/**
 * @brief  Funci�n para darle formato al mensaje bluetooth.
 *
 * @return    -
 *
 * Se le asigna el formato adecuado al mensaje para que pueda ser
 * enviado por bluetooth.
*/
void BLUETOOTH_formatear_mensaje(MENSAJEClass mensaje){
	int contador_trama = 4; /*Usado para copiar los datos en una posicion del buffer*/

	/*Tipo*/
	gs_ba_envio[0] = mensaje.tipo + gs_i_cero_ascii;
	gs_ba_envio[1] = gs_uc_caracter_barra;
	/*Latitud*/
	gs_ba_envio[2] = mensaje.posicion.latitud + gs_i_cero_ascii;
	gs_ba_envio[3] = gs_uc_caracter_punto;
	BLUETOOTH_calcular_llevada(mensaje.posicion.latitud_grado, &contador_trama);
	gs_ba_envio[contador_trama] = gs_uc_caracter_punto;
	contador_trama++;
	BLUETOOTH_calcular_llevada(mensaje.posicion.latitud_minuto, &contador_trama);
	gs_ba_envio[contador_trama] = gs_uc_caracter_punto;
	contador_trama++;
	BLUETOOTH_calcular_llevada(mensaje.posicion.latitud_segundo, &contador_trama);
	gs_ba_envio[contador_trama] = gs_uc_caracter_barra;
	contador_trama++;
	/*Longitud*/
	gs_ba_envio[contador_trama] = mensaje.posicion.longitud + gs_i_cero_ascii;
	contador_trama++;
	gs_ba_envio[contador_trama] = gs_uc_caracter_punto;
	contador_trama++;
	BLUETOOTH_calcular_llevada(mensaje.posicion.longitud_grado, &contador_trama);
	gs_ba_envio[contador_trama] = gs_uc_caracter_punto;
	contador_trama++;
	BLUETOOTH_calcular_llevada(mensaje.posicion.longitud_minuto, &contador_trama);
	gs_ba_envio[contador_trama] = gs_uc_caracter_punto;
	contador_trama++;
	BLUETOOTH_calcular_llevada(mensaje.posicion.longitud_segundo, &contador_trama);
	gs_ba_envio[contador_trama] = gs_uc_caracter_barra;
	contador_trama++;
	/*Valor*/
	BLUETOOTH_insertar_info_envio(mensaje, &contador_trama);
	/*Fin de trama (dolar)*/
	gs_ba_envio[contador_trama] = 36;
	contador_trama++;

	gs_i_tamano = contador_trama;
}
/**
 * @brief  Funci�n para calcular la llevada de los datos a
 * enviar mediante bluetooth.
 *
 * @return    -
 *
 * Se calcula la llevada de ciertos datos (latitud, longitud,
 * etc.) porque se env�an utilizando la codificacion ASCII.
*/
void BLUETOOTH_calcular_llevada(int dato, int *contador_trama){
	int llevada_centena = 0; /*Llevada de la centena*/
	int llevada_decena = 0; /*Llevada de la decena*/

	if(dato < 10){
		gs_ba_envio[(*contador_trama)] = dato + gs_i_cero_ascii;
		(*contador_trama)++;
	}else if(dato < 100){
		while(dato >= 10){
			llevada_decena++;
			dato -= 10;
		}
		gs_ba_envio[(*contador_trama)] = llevada_decena + gs_i_cero_ascii;
		(*contador_trama)++;
		gs_ba_envio[(*contador_trama)] = dato + gs_i_cero_ascii;
		(*contador_trama)++;
	}else{
		llevada_centena++;
		dato -= 100;
		while(dato >= 10){
			llevada_decena++;
			dato -= 10;
		}
		gs_ba_envio[(*contador_trama)] = llevada_centena + gs_i_cero_ascii;
		(*contador_trama)++;
		gs_ba_envio[(*contador_trama)] = llevada_decena + gs_i_cero_ascii;
		(*contador_trama)++;
		gs_ba_envio[(*contador_trama)] = dato + gs_i_cero_ascii;
		(*contador_trama)++;
	}
}
/**
 * @brief  Funci�n para cargar el valor que tiene el mensaje.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habr� que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
void BLUETOOTH_insertar_info_envio(MENSAJEClass mensaje, int *contador_trama){
	switch(mensaje.tipo){
		case TRAFICO_DENSO:
			gs_ba_envio[(*contador_trama)] = mensaje.valor.trafico_denso.direccion + gs_i_cero_ascii;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = gs_uc_caracter_barra;
			(*contador_trama)++;
			BLUETOOTH_calcular_llevada(mensaje.valor.trafico_denso.velocidad, contador_trama);
			break;
		case OBRAS:
			gs_ba_envio[(*contador_trama)] = mensaje.valor.obra.direccion + gs_i_cero_ascii;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = gs_uc_caracter_barra;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = mensaje.valor.obra.carretera_cortada + gs_i_cero_ascii;
			(*contador_trama)++;
			break;
		case VEHICULO_NO_VISIBLE:
			gs_ba_envio[(*contador_trama)] = mensaje.valor.vehiculo_no_visible.direccion + gs_i_cero_ascii;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = gs_uc_caracter_barra;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = mensaje.valor.vehiculo_no_visible.velocidad;
			(*contador_trama)++;
			break;
		case POCA_VISIBILIDAD:
			gs_ba_envio[(*contador_trama)] = mensaje.valor.poca_visibilidad.tipo + gs_i_cero_ascii;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = gs_uc_caracter_barra;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = mensaje.valor.poca_visibilidad.gravedad + gs_i_cero_ascii;
			(*contador_trama)++;
			break;
		case ESTADO_CARRETERA:
			gs_ba_envio[(*contador_trama)] = mensaje.valor.estado_carretera.tipo + gs_i_cero_ascii;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = gs_uc_caracter_barra;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = mensaje.valor.estado_carretera.direccion + gs_i_cero_ascii;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = gs_uc_caracter_barra;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = mensaje.valor.estado_carretera.gravedad + gs_i_cero_ascii;
			(*contador_trama)++;
			break;
		case ACCIDENTE_CARRETERA:
			gs_ba_envio[(*contador_trama)] = mensaje.valor.accidente_carretera.direccion + gs_i_cero_ascii;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = gs_uc_caracter_barra;
			(*contador_trama)++;
			gs_ba_envio[(*contador_trama)] = mensaje.valor.accidente_carretera.carretera_cortada + gs_i_cero_ascii;
			(*contador_trama)++;
			break;
	}
}
/**
 * @brief  Funci�n para enviar el mensaje de emparejamiento
 * de bluetooth.
 *
 * @return    -
 *
 * Se env�a la respuesta al comando de emparejamiento enviado
 * por la aplicacion Android para que los dos dispositivos puedan
 * comunicarse entre ellos.
*/
void BLUETOOTH_enviar_emparejamiento(void){
	uint8_t mensaje_emparejamiento[32]; /*Buffer donde se guarda el mensaje a enviar*/
	int contador = 0; /*Contador usado para copiar los datos principales*/

	//SSP CONFIRM e8:92:a4:45:b4:02 OK

	while(contador < 29){
		mensaje_emparejamiento[contador] = gs_ba_recibido[contador];
		contador++;
	}

	mensaje_emparejamiento[contador] = ' ';
	contador++;
	mensaje_emparejamiento[contador] = 'O';
	contador++;
	mensaje_emparejamiento[contador] = 'K';
	contador++;

	UART_send(gs_i_puerto_bluetooth, mensaje_emparejamiento, &contador);
}
/**
 * @brief  Funci�n para vaciar el buffer de recepcion.
 *
 * @return    -
 *
 * Vacia el buffer de recepcion de datos.
*/
void BLUETOOTH_vaciar_buffer_entrada(void){
	int contador; /*Contador usado para vaciar el buffer*/

	for(contador = 0; contador < 255; contador++){
		gs_ba_recibido[contador] = 0;
	}
}
/**
 * @brief  Funci�n para vaciar el buffer de envio.
 *
 * @return    -
 *
 * Vacia el buffer de envio de datos.
*/
void BLUETOOTH_vaciar_buffer_salida(void){
	int contador; /*Contador usado para vaciar el buffer*/

	for(contador = 0; contador < 255; contador++){
		gs_ba_envio[contador] = 0;
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
