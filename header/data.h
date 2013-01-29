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
enum tipo_sensor{LUMINOSIDAD, PRECIPITACION, LIQUIDO_CARRETERA, TEMPERATURA, VELOCIDAD, S_OBRAS /*TODO: puede que no corresponda*/};
enum hemisferio{NORTE, SUR, ESTE, OESTE};
enum tipo_mensaje{TRAFICO_DENSO, OBRAS, VEHICULO_NO_VISIBLE, POCA_VISIBILIDAD, ESTADO_CARRETERA, ACCIDENTE_CARRETERA};
enum tipo_poca_visibilidad{NIEVE, LLUVIA, NIEBLA};
enum tipo_estado_carretera{HIELO, ACEITE, BARRO, AGUA, DETERIORO};

/*
 * Estructura propia existente en trafico denso
 */
typedef struct s_TRAFICO_DENSOCls{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int velocidad;
}TRAFICO_DENSOClass;
/*
 * Estructura propia existente en obras
 */
typedef struct s_OBRASCls{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int carretera_cortada;
}OBRASClass;
/*
 * Estructura propia existente en vehiculo no visible
 */
typedef struct s_VEHICULO_NO_VISIBLECls{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int velocidad;
}VEHICULO_NO_VISIBLEClass;
/*
 * Estructura propia existente en poca visibilidad
 */
typedef struct s_POCA_VISIBILIDADCls{
	enum tipo_poca_visibilidad tipo;
	int gravedad;
}POCA_VISIBILIDADClass;
/*
 * Estructura propia existente en estado carretera
 */
typedef struct s_ESTADO_CARRETERACls{
	enum tipo_estado_carretera tipo;
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int gravedad;
}ESTADO_CARRETERAClass;
/*
 * Estructura propia existente en accidente carretera
 */
typedef struct s_ACCIDENTE_CARRETERACls{
	int direccion;			//(0=Km carretera de menor a mayor / 1=Km de mayor a menor )
	int carretera_cortada;
}ACCIDENTE_CARRETERAClass;
/*
 * Estructura que guarda los valores del mensaje
 */
typedef struct s_DATO_MENSAJECls{
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
typedef struct s_COORDENADASCls{
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
typedef struct s_SENSORCls{
	enum tipo_sensor tipo;
	int hora;				//TODO: el tipo se podria cambiar en un futuro
	COORDENADASClass posicion;
	float valor;
}SENSORClass;
/*
 * Estructura que guarda los mensajes recibidos de otros controladores
 */
typedef struct s_MENSAJECls{
	enum tipo_mensaje tipo;
	int id;
	int hora;				//TODO: el tipo se podria cambiar en un futuro
	COORDENADASClass posicion;
	int ttl;				//numero de veces que se puede reenviar
	DATO_MENSAJEClass valor;
}MENSAJEClass;
/*
 * Estructura que guarda los datos a enviar a la interfaz grafica
 */
typedef struct s_INTERFAZ_GRAFICACls{
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
