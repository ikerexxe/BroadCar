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
#include "hw_types.h"
#include "data.h"
#include "uartDrv.h"
/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
MENSAJEClass valor_mensaje(SENSORClass sensor, MENSAJEClass mensaje);
int valor_envio(MENSAJEClass mensaje, int contador);
MENSAJEClass tipo_mensaje(MENSAJEClass mensaje, int tipo);
void formatear_mensaje(MENSAJEClass mensaje);
int calcular_checksum(uint8_t mensaje[]);
void calcular_tamano_mensaje(MENSAJEClass mensaje);
MENSAJEClass tratar_mensaje(uint8_t recibido[]);
MENSAJEClass valor_recibido(MENSAJEClass mensaje, uint8_t recibido[]);
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern int g_i_mi_id;
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
static int g_i_numero_cabecera = 17; /*Numero de elementos que tiene la cabecera de la trama zigbee*/
static unsigned char g_uc_caracter_vacio = 0x00; /*Byte vacio = 00000000*/
static unsigned char g_uc_caracter_lleno = 255; /*Byte lleno = 11111111*/
static int g_i_puerto_zigbee = 0; /*Puerto UART que se usa para la comunicacion con el modulo zigbee*/
static int g_i_tamano = 0; /*Tama�o del mensaje*/
static uint8_t g_ba_envio[255]; /*Mensaje a enviar en formato byte*/
static uint8_t g_ba_length[2]; /*Tama�o del mensaje que se usa para enviar en la cabecera de la trama*/
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
void BROADCAR_inicializacion_zigbee(){
	openUART(g_i_puerto_zigbee);
}
/**
 * @brief  Funci�n para enviar un mensaje mediante zigbee.
 *
 * @return    -
 *
 * Recoge los datos del sensor, le da un formato adecuado para enviar
 * mediante zigbee y lo env�a.
*/
void BROADCAR_enviar_mensaje(SENSORClass sensor){
	MENSAJEClass mensaje;

	mensaje = tipo_mensaje(mensaje, sensor.tipo);
	mensaje.id = g_i_mi_id;
	mensaje.hora = sensor.hora;
	mensaje.posicion = sensor.posicion;
	mensaje.ttl = 3;
	mensaje = valor_mensaje(sensor, mensaje);

	formatear_mensaje(mensaje);
	sendUART(g_i_puerto_zigbee, g_ba_envio, &g_i_tamano);
}
/**
 * @brief  Funci�n que mira si se ha recibido un mensaje por zigbee.
 *
 * @return    Si se ha recibido un mensaje.
 *
 * Mira si se han recibido m�s de 33 bytes mediante zigbee, indicativo
 * de que se ha recibido el mensaje completo.
*/
tBoolean BROADCAST_hay_mensaje(void){
	int numero_elementos = 0; /*Numero de elementos que hay en el buffer de software*/
	tBoolean completo = false; /*Si se ha recibido un mensaje completo*/

	numero_elementos = nElementosIn(g_i_puerto_zigbee);
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
MENSAJEClass BROADCAST_recibir_mensaje(void){
	MENSAJEClass mensaje; /*Estructura para guardar el cuerpo del mensaje*/
	uint8_t temporal[255]; /*Guarda el mensaje temporalmente*/
	uint8_t recibido[255]; /*Guarda la trama completa del mensaje*/
	int numero_recibido = 3; /*Numero de bytes a coger del buffer intermedio*/
	int checksum; /*Checksum del mensaje recibido*/
	int contador = 0; /*Contador para trasladar los datos de temporal a recibido*/

	recvUART(g_i_puerto_zigbee, temporal, &numero_recibido);
	for(contador = 0; contador < numero_recibido; contador++){
		recibido[contador] = temporal[contador];
	}
	numero_recibido = 0;
	while(recibido[1] > 255){
		numero_recibido += 255;
		recibido[1] -= 255;
	}
	numero_recibido += recibido[2];
	recvUART(g_i_puerto_zigbee, temporal, &numero_recibido);
	for(contador = 0; contador < numero_recibido; contador++){
		recibido[contador+3] = temporal[contador];
	}
	checksum = calcular_checksum(recibido);
	if(checksum == recibido[numero_recibido + 4]){
		mensaje = tratar_mensaje(recibido); //TODO: FALLA AQUIIIIIIIII!!!!!!!!!!!!
		if(mensaje.id != g_i_mi_id){ //TODO: habria que mirar tambien si anteriormente se ha recibido ese mensaje
			//Si se puede reenviamos el mensaje
			if(mensaje.ttl > 0){
				mensaje.ttl--;
				formatear_mensaje(mensaje);
				sendUART(g_i_puerto_zigbee, g_ba_envio, &g_i_tamano);
			}
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
	mensaje = valor_recibido(mensaje, recibido); //TODO: FALLA AQUIIIIIIIII!!!!!!!!!!!!
//	switch(mensaje.tipo){
//		case TRAFICO_DENSO:
//			mensaje.valor.trafico_denso.direccion = recibido[30];
//			mensaje.valor.trafico_denso.velocidad = recibido[31];
//			break;
//		case OBRAS:
//			mensaje.valor.obra.direccion = recibido[30];
//			mensaje.valor.obra.carretera_cortada = recibido[31];
//			break;
//		case VEHICULO_NO_VISIBLE:
//			mensaje.valor.vehiculo_no_visible.direccion = recibido[30];
//			mensaje.valor.vehiculo_no_visible.velocidad = recibido[31];
//			break;
//		case POCA_VISIBILIDAD:
//			mensaje.valor.poca_visibilidad.tipo = recibido[30];
//			mensaje.valor.poca_visibilidad.gravedad = recibido[31];
//			break;
//		case ESTADO_CARRETERA:
//			mensaje.valor.estado_carretera.tipo = recibido[30];
//			mensaje.valor.estado_carretera.direccion = recibido[31];
//			mensaje.valor.estado_carretera.gravedad = recibido[32];
//			break;
//		case ACCIDENTE_CARRETERA:
//			mensaje.valor.accidente_carretera.direccion = recibido[30];
//			mensaje.valor.accidente_carretera.carretera_cortada = recibido[31];
//			break;
//	}

	return mensaje;
}
/**
 * @brief  Funci�n para ponerle el tipo al mensaje.
 *
 * @return    -
 *
 * Dependiendo de que sensor recoja el dato se le asigna
 * un tipo al mensaje.
*/
MENSAJEClass tipo_mensaje(MENSAJEClass mensaje, int tipo){
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

	return mensaje;
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

	g_ba_envio[0] = 0x7E;
	g_ba_envio[1] = g_ba_length[0];
	g_ba_envio[2] = g_ba_length[1];
	g_ba_envio[3] = 0x10;
	g_ba_envio[4] = 0x01;
	/*Direccion de destino: broadcast*/
	g_ba_envio[5] = g_uc_caracter_vacio;
	g_ba_envio[6] = g_uc_caracter_vacio;
	g_ba_envio[7] = g_uc_caracter_vacio;
	g_ba_envio[8] = g_uc_caracter_vacio;
	g_ba_envio[9] = g_uc_caracter_vacio;
	g_ba_envio[10] = g_uc_caracter_vacio;
	g_ba_envio[11] = g_uc_caracter_lleno;
	g_ba_envio[12] = g_uc_caracter_lleno;
	/*Red de destino*/
	g_ba_envio[13] = g_uc_caracter_lleno;
	g_ba_envio[14] = 0xFE;
	/*Otros datos*/
	g_ba_envio[15] = g_uc_caracter_vacio;
	g_ba_envio[16] = g_uc_caracter_vacio;
	/*Cuerpo del mensaje*/
	g_ba_envio[17] = mensaje.tipo;
	g_ba_envio[18] = mensaje.id;
	while(mensaje.hora > 255){
		llevada_hora++;
		mensaje.hora -= 255;
	}
	g_ba_envio[19] = llevada_hora;
	g_ba_envio[20] = mensaje.hora;
	g_ba_envio[21] = mensaje.posicion.latitud;
	g_ba_envio[22] = mensaje.posicion.latitud_grado;
	g_ba_envio[23] = mensaje.posicion.latitud_minuto;
	g_ba_envio[24] = mensaje.posicion.latitud_segundo;
	g_ba_envio[25] = mensaje.posicion.longitud;
	g_ba_envio[26] = mensaje.posicion.longitud_grado;
	g_ba_envio[27] = mensaje.posicion.longitud_minuto;
	g_ba_envio[28] = mensaje.posicion.longitud_segundo;
	g_ba_envio[29] = mensaje.ttl;
	contador = valor_envio(mensaje, 30);
	/*Checksum*/
	g_ba_envio[contador] = calcular_checksum(g_ba_envio);
}
/**
 * @brief  Funci�n para cargar el valor que se ha recogido del sensor.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habr� que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
MENSAJEClass valor_mensaje(SENSORClass sensor, MENSAJEClass mensaje){
	switch(sensor.tipo){
		case LUMINOSIDAD:
			mensaje.valor.poca_visibilidad.tipo =  NIEBLA;
			mensaje.valor.poca_visibilidad.gravedad = sensor.valor;
			g_i_tamano = g_i_numero_cabecera + 16;
			break;
		case PRECIPITACION:
			mensaje.valor.poca_visibilidad.tipo = LLUVIA;
			mensaje.valor.poca_visibilidad.gravedad = sensor.valor;
			g_i_tamano = g_i_numero_cabecera + 16;
			break;
		case LIQUIDO_CARRETERA:
			mensaje.valor.estado_carretera.tipo = ACEITE;
			mensaje.valor.estado_carretera.direccion = 0;
			mensaje.valor.estado_carretera.gravedad = sensor.valor;
			g_i_tamano = g_i_numero_cabecera + 17;
			break;
		case TEMPERATURA:
			mensaje.valor.estado_carretera.tipo = HIELO;
			mensaje.valor.estado_carretera.direccion = 0;
			mensaje.valor.estado_carretera.gravedad = 5;
			g_i_tamano = g_i_numero_cabecera + 17;
			break;
		case VELOCIDAD:
			mensaje.valor.trafico_denso.direccion = 0;
			mensaje.valor.trafico_denso.velocidad = sensor.valor;
			g_i_tamano = g_i_numero_cabecera + 16;
			break;
		case SENSOR_OBRAS:
			mensaje.valor.obra.direccion = 0;
			mensaje.valor.obra.carretera_cortada = 1;
			g_i_tamano = g_i_numero_cabecera + 16;
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
int valor_envio(MENSAJEClass mensaje, int contador){
	switch(mensaje.tipo){
		case TRAFICO_DENSO:
			g_ba_envio[contador] = mensaje.valor.trafico_denso.direccion;
			contador++;
			g_ba_envio[contador] = mensaje.valor.trafico_denso.velocidad;
			contador++;
			break;
		case OBRAS:
			g_ba_envio[contador] = mensaje.valor.obra.direccion;
			contador++;
			g_ba_envio[contador] = mensaje.valor.obra.carretera_cortada;
			contador++;
			break;
		case VEHICULO_NO_VISIBLE:
			g_ba_envio[contador] = mensaje.valor.vehiculo_no_visible.direccion;
			contador++;
			g_ba_envio[contador] = mensaje.valor.vehiculo_no_visible.velocidad;
			contador++;
			break;
		case POCA_VISIBILIDAD:
			g_ba_envio[contador] = mensaje.valor.poca_visibilidad.tipo;
			contador++;
			g_ba_envio[contador] = mensaje.valor.poca_visibilidad.gravedad;
			contador++;
			break;
		case ESTADO_CARRETERA:
			g_ba_envio[contador] = mensaje.valor.estado_carretera.tipo;
			contador++;
			g_ba_envio[contador] = mensaje.valor.estado_carretera.direccion;
			contador++;
			g_ba_envio[contador] = mensaje.valor.estado_carretera.gravedad;
			contador++;
			break;
		case ACCIDENTE_CARRETERA:
			g_ba_envio[contador] = mensaje.valor.accidente_carretera.direccion;
			contador++;
			g_ba_envio[contador] = mensaje.valor.accidente_carretera.carretera_cortada;
			contador++;
			break;
	}
//		case LUMINOSIDAD:
//			envio[contador] = mensaje.valor.poca_visibilidad.tipo;
//			contador++;
//			envio[contador] = mensaje.valor.poca_visibilidad.gravedad;
//			contador++;
//			break;
//		case PRECIPITACION:
//			envio[contador] = mensaje.valor.poca_visibilidad.tipo;
//			contador++;
//			envio[contador] = mensaje.valor.poca_visibilidad.gravedad;
//			contador++;
//			break;
//		case LIQUIDO_CARRETERA:
//			envio[contador] = mensaje.valor.estado_carretera.tipo;
//			contador++;
//			envio[contador] = mensaje.valor.estado_carretera.direccion;
//			contador++;
//			envio[contador] = mensaje.valor.estado_carretera.gravedad;
//			contador++;
//			break;
//		case TEMPERATURA:
//			envio[contador] = mensaje.valor.estado_carretera.tipo;
//			contador++;
//			envio[contador] = mensaje.valor.estado_carretera.direccion;
//			contador++;
//			envio[contador] = mensaje.valor.estado_carretera.gravedad;
//			contador++;
//			break;
//		case VELOCIDAD:
//			envio[contador] = mensaje.valor.trafico_denso.direccion;
//			contador++;
//			envio[contador] = mensaje.valor.trafico_denso.velocidad;
//			contador++;
//			break;
//		case SENSOR_OBRAS:
//			envio[contador] = mensaje.valor.trafico_denso.direccion;
//			contador++;
//			envio[contador] = mensaje.valor.obra.carretera_cortada;
//			contador++;
//			break;
//	}

	return contador;
}
/**
 * @brief  Funci�n para guardar el valor que se recibe.
 *
 * @return    -
 *
 * Dependiendo del tipo de mensaje que sea habr� que tratar ese valor
 * para que tenga sentido en la interfaz de destino.
*/
MENSAJEClass valor_recibido(MENSAJEClass mensaje, uint8_t recibido[]){
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
 * @brief  Funci�n para calcular el tama�o del mensaje.
 *
 * @return    -
 *
 * Calcula el tama�o del mensaje y asigna los valores de forma
 * que sean comprensibles en la trama zigbee.
*/
void calcular_tamano_mensaje(MENSAJEClass mensaje){
	int unidad_tamano = g_i_tamano - 4; /*Calcular el tama�o del mensaje en un byte*/
	int llevada_tamano = 0; /*Llevada para calcular el tama�o del mensaje en m�s de un byte*/

	while(unidad_tamano > 255){
		unidad_tamano -= 255;
		llevada_tamano++;
	}

	g_ba_length[0] = llevada_tamano;
	g_ba_length[1] = unidad_tamano;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
