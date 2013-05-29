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
#include <string.h>
#include "broadcar.h"
#include "zigbee.h"
#include "broadcar.h"
#ifdef STELLARIS
#include "hw_types.h"
#include "displayStellaris.h"
#include "uartDrvStellaris.h"
#else
#include "displaySpartan.h"
#include "uartDrvSpartan.h"
#include "xintc.h"
#include "xintc_l.h"
#include "xil_exception.h"
#endif
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
boolean hay_mensaje(void);
MENSAJEClass recibir_mensaje(void);
MENSAJEClass tratar_mensaje(uint8_t recibido[]);
MENSAJEClass extraer_info_recibido(MENSAJEClass mensaje, uint8_t recibido[]); //Cambiar nombre
boolean recibido_anteriormente(MENSAJEClass mensaje);
void reenviar_mensaje(MENSAJEClass mensaje);
void borrar_mensaje_lista(void);
void calcular_tamano_mensaje(MENSAJEClass mensaje);
void formatear_mensaje(MENSAJEClass mensaje);
int calcular_checksum(uint8_t mensaje[]);
MENSAJEClass insertar_info_mensaje(SENSORClass sensor, MENSAJEClass mensaje);
int insertar_info_envio(MENSAJEClass mensaje, int contador);
MENSAJEClass insertar_tipo_mensaje(MENSAJEClass mensaje, int tipo);
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
//TODO: no hay
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
MENSAJEClass g_cm_mensajes[MAX_MENSAJES]; /*Mensajes que se han recibido*/
static int gs_i_numero_cabecera = 17; /*Numero de elementos que tiene la cabecera de la trama zigbee*/
static unsigned char gs_uc_caracter_vacio = 0x00; /*Byte vacio = 00000000*/
static unsigned char gs_uc_caracter_lleno = 255; /*Byte lleno = 11111111*/
static int gs_i_puerto_zigbee = 1; /*Puerto UART que se usa para la comunicacion con el modulo zigbee*/
static int gs_i_tamano = 0; /*Tama�o del mensaje*/
static uint8_t gs_ba_envio[255]; /*Mensaje a enviar en formato byte*/
static uint8_t gs_ba_length[2]; /*Tama�o del mensaje que se usa para enviar en la cabecera de la trama*/
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
 * Inicializa el puerto UART para poder comunicarse con el modulo zigbee
*/
void ZIGBEE_inicializacion(){
	UART_open(gs_i_puerto_zigbee);
}
/**
 * @brief  Funcion que ejecuta el automata de los mensajes
*/
void ZIGBEE_recepcion_mensajes(void){
	unsigned char * pantalla;
	MENSAJEClass m_mensaje; /*Cuerpo del mensaje que se recibe por zigbee*/
	boolean b_mensaje = false; /*Si se ha recibido un mensaje completo*/

	b_mensaje = hay_mensaje();
	if(b_mensaje){
		m_mensaje = recibir_mensaje();
		if(m_mensaje.id != NULL){
			if(g_i_numero_mensaje < (MAX_MENSAJES / 2)){
				g_cm_mensajes[g_i_numero_mensaje] = m_mensaje;
				g_i_numero_mensaje++;
			}else{
				borrar_mensaje_lista();
				g_cm_mensajes[(g_i_numero_mensaje - 1)] = m_mensaje;
			}
			//TODO: enviar mendiante bluetooth
			pantalla = malloc(sizeof(unsigned char) * 20);
			switch(m_mensaje.tipo){
				case TRAFICO_DENSO:
					sprintf(pantalla, "M_TR_DENSO %d", m_mensaje.id);
					gs_i_tamano = gs_i_numero_cabecera + 16;
					break;
				case OBRAS:
					sprintf(pantalla, "M_OBRAS %d", m_mensaje.id);
					gs_i_tamano = gs_i_numero_cabecera + 16;
					break;
				case VEHICULO_NO_VISIBLE:
					sprintf(pantalla, "M_NO_VIS %d", m_mensaje.id);
					gs_i_tamano = gs_i_numero_cabecera + 16;
					break;
				case POCA_VISIBILIDAD:
					sprintf(pantalla, "M_POCA_VIS %d", m_mensaje.id);
					gs_i_tamano = gs_i_numero_cabecera + 16;
					break;
				case ESTADO_CARRETERA:
					sprintf(pantalla, "M_EST_CAR %d", m_mensaje.id);
					gs_i_tamano = gs_i_numero_cabecera + 17;
					break;
				case ACCIDENTE_CARRETERA:
					sprintf(pantalla, "M_ACC_CAR %d", m_mensaje.id);
					gs_i_tamano = gs_i_numero_cabecera + 16;
					break;
			}
			DISPLAY_escribir(pantalla);
			reenviar_mensaje(m_mensaje);
		}
	}
}
/**
 * @brief  Funci�n para enviar un mensaje mediante zigbee.
 *
 * @return    -
 *
 * Recoge los datos del sensor, le da un formato adecuado para enviar
 * mediante zigbee y lo env�a.
*/
void ZIGBEE_enviar_mensaje(SENSORClass sensor){
	MENSAJEClass mensaje;

	mensaje = insertar_tipo_mensaje(mensaje, sensor.tipo);
	mensaje.id = g_i_mi_id;
	mensaje.hora = sensor.hora;
	mensaje.posicion = sensor.posicion;
	mensaje.ttl = 3;
	mensaje = insertar_info_mensaje(sensor, mensaje);

	formatear_mensaje(mensaje);
	UART_send(gs_i_puerto_zigbee, gs_ba_envio, &gs_i_tamano);
}
/**
 * @brief  Funci�n que mira si se ha recibido un mensaje por zigbee.
 *
 * @return    Si se ha recibido un mensaje.
 *
 * Mira si se han recibido m�s de 33 bytes mediante zigbee, indicativo
 * de que se ha recibido el mensaje completo.
*/
boolean hay_mensaje(void){
	int numero_elementos = 0; /*Numero de elementos que hay en el buffer de software*/
	boolean completo = false; /*Si se ha recibido un mensaje completo*/

	numero_elementos = UART_nElementosIn(gs_i_puerto_zigbee);
	if(numero_elementos >= 33){
		completo = true;
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
MENSAJEClass recibir_mensaje(void){
	MENSAJEClass mensaje; /*Estructura para guardar el cuerpo del mensaje*/
	uint8_t temporal[255]; /*Guarda el mensaje temporalmente*/
	uint8_t recibido[255]; /*Guarda la trama completa del mensaje*/
	int numero_recibido = 3; /*Numero de bytes a coger del buffer intermedio*/
	int checksum; /*Checksum del mensaje recibido*/
	int contador = 0; /*Contador para trasladar los datos de temporal a recibido*/

	UART_recv(gs_i_puerto_zigbee, temporal, &numero_recibido);
	for(contador = 0; contador < numero_recibido; contador++){
		recibido[contador] = temporal[contador];
	}
	numero_recibido = 0;
	numero_recibido += recibido[2] + 1;
	UART_recv(gs_i_puerto_zigbee, temporal, &numero_recibido);
	for(contador = 0; contador < numero_recibido; contador++){
		recibido[contador+3] = temporal[contador];
	}
	checksum = calcular_checksum(recibido);
	if(checksum == recibido[numero_recibido + 2]){
		mensaje = tratar_mensaje(recibido);
		if(mensaje.id == g_i_mi_id || recibido_anteriormente(mensaje)){
			mensaje.id = NULL;
		}
	}else{
		//TODO: ERROR!!!!!
	}

	return mensaje;
}
/**
 * @brief  Funci�n que trata el mensaje recibido por zigbee.
 *
 * @return    El cuerpo del mensaje.
 *
 * Guarda los datos del cuerpo del mensaje.
*/
MENSAJEClass tratar_mensaje(uint8_t recibido[]){
	MENSAJEClass mensaje;

	mensaje.tipo = recibido[17];
	mensaje.id = recibido[18];
	mensaje.hora = recibido[19] * 255 + recibido[20];
	mensaje.posicion.latitud = recibido[21];
	mensaje.posicion.latitud_grado = recibido[22];
	mensaje.posicion.latitud_minuto = recibido[23];
	mensaje.posicion.latitud_segundo = recibido[24];
	mensaje.posicion.longitud = recibido[25];
	mensaje.posicion.longitud_grado = recibido[26];
	mensaje.posicion.longitud_minuto = recibido[27];
	mensaje.posicion.longitud_segundo = recibido[28];
	mensaje.ttl = recibido[29];
	mensaje = extraer_info_recibido(mensaje, recibido);

	return mensaje;
}
/**
 * @brief  Funci�n para guardar el valor que se recibe.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habr� que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
MENSAJEClass extraer_info_recibido(MENSAJEClass mensaje, uint8_t recibido[]){
	switch(mensaje.tipo){
		case TRAFICO_DENSO:
			mensaje.valor.trafico_denso.direccion = recibido[30];
			mensaje.valor.trafico_denso.velocidad = recibido[31];
			break;
		case OBRAS:
			mensaje.valor.obra.direccion = recibido[30];
			mensaje.valor.obra.carretera_cortada = recibido[31];
			break;
		case VEHICULO_NO_VISIBLE:
			mensaje.valor.vehiculo_no_visible.direccion = recibido[30];
			mensaje.valor.vehiculo_no_visible.velocidad = recibido[31];
			break;
		case POCA_VISIBILIDAD:
			mensaje.valor.poca_visibilidad.tipo = recibido[30];
			mensaje.valor.poca_visibilidad.gravedad = recibido[31];
			break;
		case ESTADO_CARRETERA:
			mensaje.valor.estado_carretera.tipo = recibido[30];
			mensaje.valor.estado_carretera.direccion = recibido[31];
			mensaje.valor.estado_carretera.gravedad = recibido[32];
			break;
		case ACCIDENTE_CARRETERA:
			mensaje.valor.accidente_carretera.direccion = recibido[30];
			mensaje.valor.accidente_carretera.carretera_cortada = recibido[31];
			break;
	}

	return mensaje;
}
/**
* @brief Funci�n que mira si el mensaje ya se ha recibido.
*
* @return Si se ha recibido con anterioridad
*
* Se recorre la lista de mensajes y se mira si el mensaje se hab�a
* recibido con anterioridad. Esto ocurre cuando el id, el tipo y la
* hora concuerdan.
*/
boolean recibido_anteriormente(MENSAJEClass mensaje){
	int contador; /*Contador para recorrer el array*/
	boolean resultado = false; /*Para saber si se ha recibido anteriormente*/

	for(contador = 0; contador < g_i_numero_mensaje; contador++){
		if(g_cm_mensajes[contador].id == mensaje.id
				&& g_cm_mensajes[contador].tipo == mensaje.tipo
				&& g_cm_mensajes[contador].hora == mensaje.hora){
			resultado = true;
			contador = g_i_numero_mensaje;
		}
	}

	return resultado;
}
/**
* @brief Funci�n para se usa para reenviar el mensaje que se ha recibido
* mediante zigbee.
*
* @return -
*
* Permite reenviar el mensaje recibido mediante zigbee siempre y cuando
* haya saltos disponibles.
*/
void reenviar_mensaje(MENSAJEClass mensaje){
	//Si se puede reenviamos el mensaje
	if(mensaje.ttl > 0){
		mensaje.ttl--;
		formatear_mensaje(mensaje);
		UART_send(gs_i_puerto_zigbee, gs_ba_envio, &gs_i_tamano);
	}
}
/**
* @brief Funci�n para se usa para borrar el primer mensaje de la lista.
*
* @return -
*
* Permite borrar el mensaje m�s antiguo de la lista de mensajes para hacer
* hueco a los nuevos mensajes.
*/
void borrar_mensaje_lista(){
	int contador = 0; /*Se usa para recorrer la lista de mensajes*/

	for(contador = 1; contador < g_i_numero_mensaje; contador++){
		g_cm_mensajes[contador - 1] = g_cm_mensajes[contador];
	}
}
/**
 * @brief  Funci�n para calcular el tama�o del mensaje.
 *
 * @return    -
 *
 * Calcula el tama�o del mensaje y asigna los valores de forma
 * que sean comprensibles en la trama zigbee.
*/
void calcular_tamano_mensaje(MENSAJEClass mensaje){
	int unidad_tamano = gs_i_tamano - 4; /*Calcular el tama�o del mensaje en un byte*/
	int llevada_tamano = 0; /*Llevada para calcular el tama�o del mensaje en m�s de un byte*/

	while(unidad_tamano > 255){
		unidad_tamano -= 255;
		llevada_tamano++;
	}

	gs_ba_length[0] = llevada_tamano;
	gs_ba_length[1] = unidad_tamano;
}
/**
 * @brief  Funci�n para darle formato al mensaje zigbee.
 *
 * @return    -
 *
 * Se le asigna la cabecera al mensaje y trata los datos del mensaje de
 * forma que tomen unos valores adecuados a la trama zigbee.
*/
void formatear_mensaje(MENSAJEClass mensaje){
	int llevada_hora = 0; /*La llevada de la hora, toma valor en caso de que la hora sea superior a 255*/
	int contador = 0; /*Contador para asignar los valores en la posicion adecuada de envio*/

	calcular_tamano_mensaje(mensaje);

	gs_ba_envio[0] = 0x7E;
	gs_ba_envio[1] = gs_ba_length[0];
	gs_ba_envio[2] = gs_ba_length[1];
	gs_ba_envio[3] = 0x10;
	gs_ba_envio[4] = 0x01;
	/*Direccion de destino: broadcast*/
	gs_ba_envio[5] = gs_uc_caracter_vacio;
	gs_ba_envio[6] = gs_uc_caracter_vacio;
	gs_ba_envio[7] = gs_uc_caracter_vacio;
	gs_ba_envio[8] = gs_uc_caracter_vacio;
	gs_ba_envio[9] = gs_uc_caracter_vacio;
	gs_ba_envio[10] = gs_uc_caracter_vacio;
	gs_ba_envio[11] = gs_uc_caracter_lleno;
	gs_ba_envio[12] = gs_uc_caracter_lleno;
	/*Red de destino*/
	gs_ba_envio[13] = gs_uc_caracter_lleno;
	gs_ba_envio[14] = 0xFE;
	/*Otros datos*/
	gs_ba_envio[15] = gs_uc_caracter_vacio;
	gs_ba_envio[16] = gs_uc_caracter_vacio;
	/*Cuerpo del mensaje*/
	gs_ba_envio[17] = mensaje.tipo;
	gs_ba_envio[18] = mensaje.id;
	while(mensaje.hora > 255){
		llevada_hora++;
		mensaje.hora -= 255;
	}
	gs_ba_envio[19] = llevada_hora;
	gs_ba_envio[20] = mensaje.hora;
	gs_ba_envio[21] = mensaje.posicion.latitud;
	gs_ba_envio[22] = mensaje.posicion.latitud_grado;
	gs_ba_envio[23] = mensaje.posicion.latitud_minuto;
	gs_ba_envio[24] = mensaje.posicion.latitud_segundo;
	gs_ba_envio[25] = mensaje.posicion.longitud;
	gs_ba_envio[26] = mensaje.posicion.longitud_grado;
	gs_ba_envio[27] = mensaje.posicion.longitud_minuto;
	gs_ba_envio[28] = mensaje.posicion.longitud_segundo;
	gs_ba_envio[29] = mensaje.ttl;
	contador = insertar_info_envio(mensaje, 30);
	/*Checksum*/
	gs_ba_envio[contador] = calcular_checksum(gs_ba_envio);
}
/**
 * @brief  Funci�n para calcular el checksum del mensaje.
 *
 * @return    -
 *
 * Calcula el checksum del mensaje
*/
int calcular_checksum(uint8_t mensaje[]){
	int contador; /*Contador que recorre el mensaje a enviar*/
	int suma = 0; /*Suma de todos los parametros del mensaje*/
	int resultado = 0; /*Resultado del checksum*/
	int tamano_mensaje = mensaje[2] + 3; /*Tama�o del mensaje a excepcion de los tres primeros elementos de la cabecera*/

	for(contador = 3; contador < tamano_mensaje; contador++){
		suma += mensaje[contador];
	}

	resultado = suma & 0xFF;
	resultado = 0xFF - resultado;

	return resultado;
}
/**
 * @brief  Funci�n para cargar el valor que se ha recogido del sensor.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habr� que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
MENSAJEClass insertar_info_mensaje(SENSORClass sensor, MENSAJEClass mensaje){
	switch(sensor.tipo){
		case LUMINOSIDAD:
			mensaje.valor.poca_visibilidad.tipo =  NIEBLA;
			if(sensor.valor > 64) mensaje.valor.poca_visibilidad.gravedad = 1;
			else if(sensor.valor > 48) mensaje.valor.poca_visibilidad.gravedad = 2;
			else if(sensor.valor > 32) mensaje.valor.poca_visibilidad.gravedad = 3;
			else if(sensor.valor > 16) mensaje.valor.poca_visibilidad.gravedad = 4;
			else mensaje.valor.poca_visibilidad.gravedad = 5;
			gs_i_tamano = gs_i_numero_cabecera + 16;
			break;
		case PRECIPITACION:
			mensaje.valor.poca_visibilidad.tipo = LLUVIA;
			mensaje.valor.poca_visibilidad.gravedad = sensor.valor;
			gs_i_tamano = gs_i_numero_cabecera + 16;
			break;
		case LIQUIDO_CARRETERA:
			mensaje.valor.estado_carretera.tipo = ACEITE;
			mensaje.valor.estado_carretera.direccion = 0;
			if(sensor.valor > 64) mensaje.valor.estado_carretera.gravedad = 1;
			else if(sensor.valor > 48) mensaje.valor.estado_carretera.gravedad = 2;
			else if(sensor.valor > 32) mensaje.valor.estado_carretera.gravedad = 3;
			else if(sensor.valor > 16) mensaje.valor.estado_carretera.gravedad = 4;
			else mensaje.valor.estado_carretera.gravedad = 5;
			gs_i_tamano = gs_i_numero_cabecera + 17;
			break;
		case TEMPERATURA:
			mensaje.valor.estado_carretera.tipo = HIELO;
			mensaje.valor.estado_carretera.direccion = 0;
			mensaje.valor.estado_carretera.gravedad = 5;
			gs_i_tamano = gs_i_numero_cabecera + 17;
			break;
		case VELOCIDAD:
			mensaje.valor.trafico_denso.direccion = 0;
			mensaje.valor.trafico_denso.velocidad = sensor.valor;
			gs_i_tamano = gs_i_numero_cabecera + 16;
			break;
		case S_OBRAS:
			mensaje.valor.obra.direccion = 0;
			if(sensor.valor > 50) mensaje.valor.obra.carretera_cortada = 0;
			else mensaje.valor.obra.carretera_cortada = 1;
			gs_i_tamano = gs_i_numero_cabecera + 16;
			break;
	}

	return mensaje;
}
/**
 * @brief  Funci�n para cargar el valor que tiene el mensaje.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habr� que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
int insertar_info_envio(MENSAJEClass mensaje, int contador){
	switch(mensaje.tipo){
		case TRAFICO_DENSO:
			gs_ba_envio[contador] = mensaje.valor.trafico_denso.direccion;
			contador++;
			gs_ba_envio[contador] = mensaje.valor.trafico_denso.velocidad;
			contador++;
			break;
		case OBRAS:
			gs_ba_envio[contador] = mensaje.valor.obra.direccion;
			contador++;
			gs_ba_envio[contador] = mensaje.valor.obra.carretera_cortada;
			contador++;
			break;
		case VEHICULO_NO_VISIBLE:
			gs_ba_envio[contador] = mensaje.valor.vehiculo_no_visible.direccion;
			contador++;
			gs_ba_envio[contador] = mensaje.valor.vehiculo_no_visible.velocidad;
			contador++;
			break;
		case POCA_VISIBILIDAD:
			gs_ba_envio[contador] = mensaje.valor.poca_visibilidad.tipo;
			contador++;
			gs_ba_envio[contador] = mensaje.valor.poca_visibilidad.gravedad;
			contador++;
			break;
		case ESTADO_CARRETERA:
			gs_ba_envio[contador] = mensaje.valor.estado_carretera.tipo;
			contador++;
			gs_ba_envio[contador] = mensaje.valor.estado_carretera.direccion;
			contador++;
			gs_ba_envio[contador] = mensaje.valor.estado_carretera.gravedad;
			contador++;
			break;
		case ACCIDENTE_CARRETERA:
			gs_ba_envio[contador] = mensaje.valor.accidente_carretera.direccion;
			contador++;
			gs_ba_envio[contador] = mensaje.valor.accidente_carretera.carretera_cortada;
			contador++;
			break;
	}

	return contador;
}
/**
 * @brief  Funci�n para ponerle el tipo al mensaje.
 *
 * @return    -
 *
 * Dependiendo de que sensor recoja el dato se le asigna
 * un tipo al mensaje.
*/
MENSAJEClass insertar_tipo_mensaje(MENSAJEClass mensaje, int tipo){
	switch(tipo){
		case LUMINOSIDAD:
			mensaje.tipo = POCA_VISIBILIDAD;
			break;
		case PRECIPITACION:
			mensaje.tipo = POCA_VISIBILIDAD;
			break;
		case LIQUIDO_CARRETERA:
			mensaje.tipo = ESTADO_CARRETERA;
			break;
		case TEMPERATURA:
			mensaje.tipo = ESTADO_CARRETERA;
			break;
		case VELOCIDAD:
			mensaje.tipo = TRAFICO_DENSO;
			break;
		case S_OBRAS:
			mensaje.tipo = OBRAS;
			break;
	}

	return mensaje;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
