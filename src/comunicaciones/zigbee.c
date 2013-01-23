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
#include <string.h>
#include "data.h"
#include "uartDrv.h"
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void valor_mensaje(SENSORClass sensor);
void tipo_mensaje(int tipo);
void formatear_mensaje(SENSORClass sensor, MENSAJEClass mensaje);
int calcular_checksum(uint8_t mensaje[]);
void calcular_tamano_mensaje(MENSAJEClass mensaje);
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
static int numero_cabecera = 17; /*Numero de elementos que tiene la cabecera de la trama zigbee*/
static unsigned char caracter_vacio = 0x00; /*Byte vacio = 00000000*/
static unsigned char caracter_lleno = 255; /*Byte lleno = 11111111*/
int puerto_zigbee = 0; /*Puerto UART que se usa para la comunicacion con el modulo zigbee*/
int tamano = 0; /*Tamaño del mensaje*/
MENSAJEClass mensaje; /*Mensaje a enviar*/
uint8_t envio[255]; /*Mensaje a enviar en formato byte*/
uint8_t length[2]; /*Tamaño del mensaje que se usa para enviar en la cabecera de la trama*/
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
 * Inicializa el puerto UART para poder comunicarse con el modulo zigbee
*/
void BROADCAR_inicializacion_zigbee(){
	openUART(puerto_zigbee);
}
/**
 * @brief  Función para enviar un mensaje mediante zigbee.
 *
 * @return    -
 *
 * Recoge los datos del sensor, le da un formato adecuado para enviar
 * mediante zigbee y lo envía.
*/
void BROADCAR_enviar_mensaje(SENSORClass sensor){
	tipo_mensaje(sensor.tipo);
	mensaje.id = 1;
	mensaje.hora = sensor.hora;
	mensaje.posicion = sensor.posicion;
	mensaje.ttl = 3;
	valor_mensaje(sensor);

	formatear_mensaje(sensor, mensaje);
	sendUART(puerto_zigbee, envio, &tamano);
}
/**
 * @brief  Función para ponerle el tipo al mensaje.
 *
 * @return    -
 *
 * Dependiendo de que sensor recoja el dato se le asigna
 * un tipo al mensaje.
*/
void tipo_mensaje(int tipo){
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
		case SENSOR_OBRAS:
			mensaje.tipo = OBRAS;
			break;
	}
}
/**
 * @brief  Función para darle formato al mensaje zigbee.
 *
 * @return    -
 *
 * Se le asigna la cabecera al mensaje y trata los datos del mensaje de
 * forma que tomen unos valores adecuados a la trama zigbee.
*/
void formatear_mensaje(SENSORClass sensor, MENSAJEClass mensaje){
	int llevada_hora = 0; /*La llevada de la hora, toma valor en caso de que la hora sea superior a 255*/
	int contador = 0; /*Contador para asignar los valores en la posicion adecuada de envio*/

	calcular_tamano_mensaje(mensaje);

	envio[0] = 0x7E;
	envio[1] = length[0];
	envio[2] = length[1];
	envio[3] = 0x10;
	envio[4] = 0x01;
	/*Direccion de destino: broadcast*/
	envio[5] = caracter_vacio;
	envio[6] = caracter_vacio;
	envio[7] = caracter_vacio;
	envio[8] = caracter_vacio;
	envio[9] = caracter_vacio;
	envio[10] = caracter_vacio;
	envio[11] = caracter_lleno;
	envio[12] = caracter_lleno;
	/*Red de destino*/
	envio[13] = caracter_lleno;
	envio[14] = 0xFE;
	/*Otros datos*/
	envio[15] = caracter_vacio;
	envio[16] = caracter_vacio;
	/*Cuerpo del mensaje*/
	envio[17] = mensaje.tipo;
	envio[18] = mensaje.id;
	while(mensaje.hora > 255){
		llevada_hora++;
		mensaje.hora -= 255;
	}
	envio[19] = llevada_hora;
	envio[20] = mensaje.hora;
	envio[21] = mensaje.posicion.latitud;
	envio[22] = mensaje.posicion.latitud_grado;
	envio[23] = mensaje.posicion.latitud_minuto;
	envio[24] = mensaje.posicion.latitud_segundo;
	envio[25] = mensaje.posicion.longitud;
	envio[26] = mensaje.posicion.longitud_grado;
	envio[27] = mensaje.posicion.longitud_minuto;
	envio[28] = mensaje.posicion.longitud_segundo;
	envio[29] = mensaje.ttl;
	contador = valor_envio(sensor, 30);
	/*Checksum*/
	envio[contador] = calcular_checksum(envio);
}
/**
 * @brief  Función para cargar el valor que se ha recogido del sensor.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habrá que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
void valor_mensaje(SENSORClass sensor){
	switch(sensor.tipo){
		case LUMINOSIDAD:
			mensaje.valor.poca_visibilidad.tipo =  NIEBLA;
			mensaje.valor.poca_visibilidad.gravedad = sensor.valor;
			tamano = numero_cabecera + 16;
			break;
		case PRECIPITACION:
			mensaje.valor.poca_visibilidad.tipo = LLUVIA;
			mensaje.valor.poca_visibilidad.gravedad = sensor.valor;
			tamano = numero_cabecera + 16;
			break;
		case LIQUIDO_CARRETERA:
			mensaje.valor.estado_carretera.tipo = ACEITE;
			mensaje.valor.estado_carretera.direccion = 0;
			mensaje.valor.estado_carretera.gravedad = sensor.valor;
			tamano = numero_cabecera + 17;
			break;
		case TEMPERATURA:
			mensaje.valor.estado_carretera.tipo = HIELO;
			mensaje.valor.estado_carretera.direccion = 0;
			mensaje.valor.estado_carretera.gravedad = 5;
			tamano = numero_cabecera + 17;
			break;
		case VELOCIDAD:
			mensaje.valor.trafico_denso.direccion = 0;
			mensaje.valor.trafico_denso.velocidad = sensor.valor;
			tamano = numero_cabecera + 16;
			break;
		case SENSOR_OBRAS:
			mensaje.valor.obra.direccion = 0;
			mensaje.valor.obra.carretera_cortada = 1;
			tamano = numero_cabecera + 16;
			break;
	}
}
/**
 * @brief  Función para cargar el valor que tiene el mensaje.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habrá que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
int valor_envio(SENSORClass sensor, int contador){
	switch(sensor.tipo){
		case LUMINOSIDAD:
			envio[contador] = mensaje.valor.poca_visibilidad.tipo;
			contador++;
			envio[contador] = mensaje.valor.poca_visibilidad.gravedad;
			contador++;
			break;
		case PRECIPITACION:
			envio[contador] = mensaje.valor.poca_visibilidad.tipo;
			contador++;
			envio[contador] = mensaje.valor.poca_visibilidad.gravedad;
			contador++;
			break;
		case LIQUIDO_CARRETERA:
			envio[contador] = mensaje.valor.estado_carretera.tipo;
			contador++;
			envio[contador] = mensaje.valor.estado_carretera.direccion;
			contador++;
			envio[contador] = mensaje.valor.estado_carretera.gravedad;
			contador++;
			break;
		case TEMPERATURA:
			envio[contador] = mensaje.valor.estado_carretera.tipo;
			contador++;
			envio[contador] = mensaje.valor.estado_carretera.direccion;
			contador++;
			envio[contador] = mensaje.valor.estado_carretera.gravedad;
			contador++;
			break;
		case VELOCIDAD:
			envio[contador] = mensaje.valor.trafico_denso.direccion;
			contador++;
			envio[contador] = mensaje.valor.trafico_denso.velocidad;
			contador++;
			break;
		case SENSOR_OBRAS:
			envio[contador] = mensaje.valor.trafico_denso.direccion;
			contador++;
			envio[contador] = mensaje.valor.obra.carretera_cortada;
			contador++;
			break;
	}

	return contador;
}
/**
 * @brief  Función para calcular el checksum del mensaje.
 *
 * @return    -
 *
 * Calcula el checksum del mensaje
*/
int calcular_checksum(uint8_t mensaje[]){
	int contador; /*Contador que recorre el mensaje a enviar*/
	int suma = 0; /*Suma de todos los parametros del mensaje*/
	int resultado = 0; /*Resultado del checksum*/
	int tamano = mensaje[2] + 3; /*Tamaño del mensaje a excepcion de los tres primeros elementos de la cabecera*/

	for(contador = 3; contador < tamano; contador++){
		suma += mensaje[contador];
	}

	resultado = suma & 0xFF;
	resultado = 0xFF - resultado;

	return resultado;
}
/**
 * @brief  Función para calcular el tamaño del mensaje.
 *
 * @return    -
 *
 * Calcula el tamaño del mensaje y asigna los valores de forma
 * que sean comprensibles en la trama zigbee.
*/
void calcular_tamano_mensaje(MENSAJEClass mensaje){
	int llevada_tamano = 0; /*Llevada para calcular el tamaño del mensaje en más de un byte*/

	while(tamano > 255){
		tamano -= 255;
		llevada_tamano++;
	}

	length[0] = llevada_tamano;
	length[1] = tamano;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/

