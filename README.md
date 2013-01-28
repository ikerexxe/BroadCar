/****************************************************************************************/
*																						 *
/										README											 /
*																						 *
/****************************************************************************************/

	EL PROGRAMA:
	
	Sirve para controlar lo que ocurre en el entorno de un vehículo y para transmitir esta
	información a otras placas y a teléfonos móviles. El control se hace con la placa
	Stellaris EKS-LM3S8962, la comunicación a otras placas con zigbee y la transmisión a
	teléfonos móviles mediante bluetooth.
	
	Para la simulación de los sensores del vehículo se usa el keypad. Para enviar
	información a otras placas un modulo zigbee que está conectado a la Stellaris
	mediante UART. También se usa una UART para enviar información al módulo Bluetooth
	quien a su vez se comunicará con el teléfono móvil. Finalmente se usa el display
	para visualizar, de forma breve, los cambios en el entorno del vehículo.
	
/****************************************************************************************/

	ORGANIZACIÓN DEL PROGRAMA:
	
	El proyecto consta de un fichero principal llamado "broadcar.c" y su fichero de 
	cabecera "broadcar.h". Las funciones correspondientes al teclado ("keypad.c"), al
	display ("display.c") y al clock ("clock.c"), han sido definidas en distintos
	ficheros, cada uno con sus respectivos nombres y ficheros de cabecera, de manera que
	pueden ser reemplazadas fácilmente en el caso de que el HW utilizado no sea
	exactamnete el mismo. También dispone de un fichero llamado "data.h" donde se definen
	las estructuras de datos del programa.
	
	Por otra parte, el fichero "display.c" hace uso del Frame Buffer definido en "frame-
	buffer.c" para hacer mejor uso de la pantalla. Este Frame Buffer es genérico, es
	decir, no está especificado para ningún display en concreto, por lo tanto, hace uso
	del fichero "displayRIT.c" para utilizar las funciones específicas del display
	RIT128x94x4.
	
	En lo que a la comunicación mediante zigbee y bluetooth se refiere, todos los ficheros
	se encuentran dentro de la carpeta llamada "comunicaciones". Dentro de esta carpeta
	están los archivos "uartDrv.c", el cual dispone de interrupciones y de buffers de
	software para no saturar el hardware propio del hardware, "zigbee.c", que se encarga
	de parsear la trama zigbee para su envío o recepción, y de "bluetooth.c", que hace lo
	mismo pero para el bluetooth. Todos estos ficheros disponen de sus archivos de
	cabecera y aparte existe el "uartConfig.h" que sirve para definir la configuración
	del driver de la UART.
	
	Los sensores de los que dispone la placa se encuentran dentro de la carpeta
	"sensores": "sensorEstado.c", "sensorObras.c", "sensorVelocidad.c" y
	"sensorVisibilidad.c". Por ahora estos sensores no están conectados a la placa por lo
	que se simulan mediante el keypad.
	
	Finalemente, la lógica del programa se hace mediante una máquina de estados. Esta
	máquina tiene definido su motor en "motorAutomatas.c" y sus estados y transiciones se
	encuentran en "automata.c".
	