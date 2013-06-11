/*****************************************************************************
**											  								**
** project :  BroadCar						 							  	**
** filename : data.h 							  					    	**
** version : 1 									  							**
** date :  	2013-01-13		 		 									    **
** 											  								**
******************************************************************************
** 											  								**
**                      											  		**
**  								  					                    **
** 											  								**
******************************************************************************
**										        							**
**VERSION HISTORY:									  						**
**----------------									  						**
**Version : 1										  						**
**Date : 2013-01-13									 					    **
**Revised by : 			                    	  							**
**Description : Original version.							  				**
*****************************************************************************/
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef DATA_H
#define DATA_H

//Listas numeradas
enum tipo_sensor{LUMINOSIDAD, PRECIPITACION, LIQUIDO_CARRETERA, TEMPERATURA, VELOCIDAD, S_OBRAS};
enum hemisferio{NORTE, SUR, ESTE, OESTE};
enum tipo_mensaje{TRAFICO_DENSO, OBRAS, VEHICULO_NO_VISIBLE, POCA_VISIBILIDAD, ESTADO_CARRETERA, ACCIDENTE_CARRETERA};
enum tipo_poca_visibilidad{NIEVE, LLUVIA, NIEBLA};
enum tipo_estado_carretera{HIELO, ACEITE, BARRO, AGUA, DETERIORO};

/*
 * Estructura propia existente en trafico denso
 */
typedef struct{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int velocidad;
}TRAFICO_DENSOClass;
/*
 * Estructura propia existente en obras
 */
typedef struct{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int carretera_cortada;
}OBRASClass;
/*
 * Estructura propia existente en vehiculo no visible
 */
typedef struct{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int velocidad;
}VEHICULO_NO_VISIBLEClass;
/*
 * Estructura propia existente en poca visibilidad
 */
typedef struct{
	enum tipo_poca_visibilidad tipo;
	int gravedad;
}POCA_VISIBILIDADClass;
/*
 * Estructura propia existente en estado carretera
 */
typedef struct{
	enum tipo_estado_carretera tipo;
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int gravedad;
}ESTADO_CARRETERAClass;
/*
 * Estructura propia existente en accidente carretera
 */
typedef struct{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int carretera_cortada;
}ACCIDENTE_CARRETERAClass;
/*
 * Estructura que guarda los valores del mensaje
 */
typedef struct{
	TRAFICO_DENSOClass trafico_denso;
	OBRASClass obra;
	VEHICULO_NO_VISIBLEClass vehiculo_no_visible;
	POCA_VISIBILIDADClass poca_visibilidad;
	ESTADO_CARRETERAClass estado_carretera;
	ACCIDENTE_CARRETERAClass accidente_carretera;
}DATO_MENSAJEClass;

/*
 * Estructura que guarda las coordenadas
 */
typedef struct{
	enum hemisferio latitud;
	int latitud_grado;
	int latitud_minuto;
	int latitud_segundo;
	enum hemisferio longitud;
	int longitud_grado;
	int longitud_minuto;
	int longitud_segundo;
}COORDENADASClass;
/*
 * Estructura que guarda los datos de un sensor
 */
typedef struct{
	enum tipo_sensor tipo;
	int hora;
	COORDENADASClass posicion;
	float valor;
}SENSORClass;
/*
 * Estructura que guarda los mensajes recibidos de otros controladores
 */
typedef struct{
	enum tipo_mensaje tipo;
	int id;
	int hora;
	COORDENADASClass posicion;
	int ttl;				//numero de veces que se puede reenviar
	DATO_MENSAJEClass valor;
}MENSAJEClass;
/*
 * Estructura que guarda los datos a enviar a la interfaz grafica
 */
typedef struct{
	enum tipo_mensaje tipo;
	COORDENADASClass posicion;
	DATO_MENSAJEClass valor;
}INTERFAZ_GRAFICAClass;
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
