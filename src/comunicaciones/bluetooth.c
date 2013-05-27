/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : zigbee.c 												**
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
** date : 2013-01-22	 											**
**Revised by : iker pedrosa											**
**Description : Originial version.									**
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
#include "hw_types.h"
#include "bluetooth.h"
#include "uartDrv.h"
#include "broadcar.h"
#include "display.h"
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
tBoolean BLUETOOTH_hay_mensaje(void);
void BLUETOOTH_recibir_mensaje(void);
void BLUETOOTH_formatear_mensaje(MENSAJEClass mensaje);
void BLUETOOTH_insertar_info_envio(MENSAJEClass mensaje);
void BLUETOOTH_enviar_emparejamiento(void);
void BLUETOOTH_vaciar_buffer(void);
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
tBoolean g_b_conectado = false;
static int gs_i_puerto_zigbee = 1; /*Puerto UART que se usa para la comunicacion con el modulo bluetooth*/
static int gs_i_tamano = 0; /*Tamaño del mensaje*/
static uint8_t gs_ba_envio[255]; /*Mensaje a enviar en formato byte*/
static uint8_t gs_ba_recibido[255]; /*Mensaje recibido en formato byte*/
static unsigned char gs_uc_caracter_barra = 0x2F;
static unsigned char gs_uc_caracter_punto = 0x2E;
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Función para inicializar el puerto UART.
 *
 * @return    -
 *
 * Inicializa el puerto UART para poder comunicarse con el modulo
 * bluetooth
*/
void BLUETOOTH_inicializacion(){
	UART_open(gs_i_puerto_zigbee);
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
	tBoolean b_mensaje = false; /*Si se ha recibido un mensaje completo*/
	unsigned char * pantalla;

	if(g_b_conectado){
		b_mensaje = BLUETOOTH_hay_mensaje();
		if(b_mensaje){
			BLUETOOTH_recibir_mensaje();
			if(gs_ba_recibido[0] == 'N'){
				g_b_conectado = false;
				BLUETOOTH_vaciar_buffer();
				pantalla = malloc(sizeof(unsigned char) * 20);
				sprintf(pantalla, "DESCONECTADO");
				DISPLAY_escribir(pantalla);
			}
		}
	}else{
		b_mensaje = BLUETOOTH_hay_mensaje();
		if(b_mensaje){
			BLUETOOTH_recibir_mensaje();
			if(gs_ba_recibido[0] == 'S'){
				BLUETOOTH_enviar_emparejamiento();
				BLUETOOTH_vaciar_buffer();
				pantalla = malloc(sizeof(unsigned char) * 20);
				sprintf(pantalla, "EMPAREJADO");
				DISPLAY_escribir(pantalla);
			}else if(gs_ba_recibido[0] == 'R'){
				g_b_conectado = true;
				BLUETOOTH_vaciar_buffer();
				pantalla = malloc(sizeof(unsigned char) * 20);
				sprintf(pantalla, "CONECTADO");
				DISPLAY_escribir(pantalla);
			}
		}
	}
}
/**
 * @brief  Función para enviar una alerta mediante bluetooth.
 *
 * @return    -
 *
 * TODO: a comentar
*/
void BLUETOOTH_enviar_mensaje(MENSAJEClass mensaje){
	BLUETOOTH_formatear_mensaje(mensaje);
	UART_send(gs_i_puerto_zigbee, gs_ba_envio, &gs_i_tamano);
}
/**
 * @brief  Función que mira si se ha recibido un mensaje por bluetooth.
 *
 * @return    Si se ha recibido un mensaje.
 *
 * Mira si se han recibido más de 33 bytes mediante bluetooth, indicativo
 * de que se ha recibido el mensaje completo.
*/
tBoolean BLUETOOTH_hay_mensaje(void){
	int numero_elementos = 0; /*Numero de elementos que hay en el buffer de software*/
	tBoolean completo = false; /*Si se ha recibido un mensaje completo*/

	numero_elementos = UART_nElementosIn(gs_i_puerto_zigbee);
	if(g_b_conectado){
		if(numero_elementos >= 20){
			completo = true;
		}
	}else{
		if(numero_elementos >= 33){
			completo = true;
		}
	}

	return completo;
}
/**
 * @brief  Función que recibe el mensaje de zigbee.
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
	UART_recv(gs_i_puerto_zigbee, temporal, &numero_recibido);
	while(temporal[0] == 0 && contador < 172){
		UART_recv(gs_i_puerto_zigbee, temporal, &numero_recibido);
		contador++;
	}

	gs_ba_recibido[0] = temporal[0];

	if(temporal[0] == 'S'){
		numero_recibido = 37;
	}else if(temporal[0] == 'R'){
		numero_recibido = 32;
	}else if(temporal[0] == 'N'){
		numero_recibido = 19;
	}

	UART_recv(gs_i_puerto_zigbee, temporal, &numero_recibido);
	for(contador = 0; contador < numero_recibido; contador++){
		gs_ba_recibido[contador+1] = temporal[contador];
	}
}
/**
 * @brief  Función para darle formato al mensaje bluetooth.
 *
 * @return    -
 *
 * Se le asigna el formato adecuado al mensaje para que pueda ser
 * enviado por bluetooth.
*/
void BLUETOOTH_formatear_mensaje(MENSAJEClass mensaje){
	/*Tipo*/
	gs_ba_envio[0] = mensaje.tipo;
	gs_ba_envio[1] = gs_uc_caracter_barra;
	/*Latitud*/
	gs_ba_envio[2] = mensaje.posicion.latitud;
	gs_ba_envio[3] = gs_uc_caracter_punto;
	gs_ba_envio[4] = mensaje.posicion.latitud_grado;
	gs_ba_envio[5] = gs_uc_caracter_punto;
	gs_ba_envio[6] = mensaje.posicion.latitud_minuto;
	gs_ba_envio[7] = gs_uc_caracter_punto;
	gs_ba_envio[8] = mensaje.posicion.latitud_segundo;
	gs_ba_envio[9] = gs_uc_caracter_barra;
	/*Longitud*/
	gs_ba_envio[10] = mensaje.posicion.longitud;
	gs_ba_envio[11] = gs_uc_caracter_punto;
	gs_ba_envio[12] = mensaje.posicion.longitud_grado;
	gs_ba_envio[13] = gs_uc_caracter_punto;
	gs_ba_envio[14] = mensaje.posicion.longitud_minuto;
	gs_ba_envio[15] = gs_uc_caracter_punto;
	gs_ba_envio[16] = mensaje.posicion.longitud_segundo;
	gs_ba_envio[17] = gs_uc_caracter_barra;
	/*Valor*/
	BLUETOOTH_insertar_info_envio(mensaje);
	//TODO: hay que añadir $ al final
}
/**
 * @brief  Función para cargar el valor que tiene el mensaje.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habrá que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
void BLUETOOTH_insertar_info_envio(MENSAJEClass mensaje){
	switch(mensaje.tipo){
		case TRAFICO_DENSO:
			gs_ba_envio[18] = mensaje.valor.trafico_denso.direccion;
			gs_ba_envio[19] = mensaje.valor.trafico_denso.velocidad;
			gs_i_tamano = 19;
			break;
		case OBRAS:
			gs_ba_envio[18] = mensaje.valor.obra.direccion;
			gs_ba_envio[19] = mensaje.valor.obra.carretera_cortada;
			gs_i_tamano = 19;
			break;
		case VEHICULO_NO_VISIBLE:
			gs_ba_envio[18] = mensaje.valor.vehiculo_no_visible.direccion;
			gs_ba_envio[19] = mensaje.valor.vehiculo_no_visible.velocidad;
			gs_i_tamano = 19;
			break;
		case POCA_VISIBILIDAD:
			gs_ba_envio[18] = mensaje.valor.poca_visibilidad.tipo;
			gs_ba_envio[19] = mensaje.valor.poca_visibilidad.gravedad;
			gs_i_tamano = 19;
			break;
		case ESTADO_CARRETERA:
			gs_ba_envio[18] = mensaje.valor.estado_carretera.tipo;
			gs_ba_envio[19] = mensaje.valor.estado_carretera.direccion;
			gs_ba_envio[20] = mensaje.valor.estado_carretera.gravedad;
			gs_i_tamano = 20;
			break;
		case ACCIDENTE_CARRETERA:
			gs_ba_envio[18] = mensaje.valor.accidente_carretera.direccion;
			gs_ba_envio[19] = mensaje.valor.accidente_carretera.carretera_cortada;
			gs_i_tamano = 19;
			break;
	}
}
//TODO: falta por comentar
void BLUETOOTH_enviar_emparejamiento(void){
	uint8_t mensaje_emparejamiento[32];
	int contador = 0;

	//SSP CONFIRM e8:92:a4:45:b4:02 OK

	while(contador < 29){
		mensaje_emparejamiento[contador] = gs_ba_recibido[contador];
		contador++;
	}

	mensaje_emparejamiento[29] = ' ';
	mensaje_emparejamiento[30] = 'O';
	mensaje_emparejamiento[31] = 'K';
	contador = 32;

	UART_send(gs_i_puerto_zigbee, mensaje_emparejamiento, &contador);
}
//TODO: comentar
void BLUETOOTH_vaciar_buffer(void){
	int contador;

	for(contador = 0; contador < 255; contador++){
		gs_ba_recibido[contador] = 0;
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
