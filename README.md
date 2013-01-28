/****************************************************************************************/
*																						 *
/										README											 /
*																						 *
/****************************************************************************************/

	EL PROGRAMA:
	
	Sirve para controlar lo que ocurre en el entorno de un veh�culo y para transmitir esta
	informaci�n a otras placas y a tel�fonos m�viles. El control se hace con la placa
	Stellaris EKS-LM3S8962, la comunicaci�n a otras placas con zigbee y la transmisi�n a
	tel�fonos m�viles mediante bluetooth.
	
	Para la simulaci�n de los sensores del veh�culo se usa el keypad. Para enviar
	informaci�n a otras placas un modulo zigbee que est� conectado a la Stellaris
	mediante UART. Tambi�n se usa una UART para enviar informaci�n al m�dulo Bluetooth
	quien a su vez se comunicar� con el tel�fono m�vil. Finalmente se usa el display
	para visualizar, de forma breve, los cambios en el entorno del veh�culo.
	
/****************************************************************************************/

	ORGANIZACI�N DEL PROGRAMA:
	
	El proyecto consta de un fichero principal llamado "broadcar.c" y su fichero de 
	cabecera "broadcar.h". Las funciones correspondientes al teclado ("keypad.c"), al
	display ("display.c") y al clock ("clock.c"), han sido definidas en distintos
	ficheros, cada uno con sus respectivos nombres y ficheros de cabecera, de manera que
	pueden ser reemplazadas f�cilmente en el caso de que el HW utilizado no sea
	exactamnete el mismo. Tambi�n dispone de un fichero llamado "data.h" donde se definen
	las estructuras de datos del programa.
	
	Por otra parte, el fichero "display.c" hace uso del Frame Buffer definido en "frame-
	buffer.c" para hacer mejor uso de la pantalla. Este Frame Buffer es gen�rico, es
	decir, no est� especificado para ning�n display en concreto, por lo tanto, hace uso
	del fichero "displayRIT.c" para utilizar las funciones espec�ficas del display
	RIT128x94x4.
	
	En lo que a la comunicaci�n mediante zigbee y bluetooth se refiere, todos los ficheros
	se encuentran dentro de la carpeta llamada "comunicaciones". Dentro de esta carpeta
	est�n los archivos "uartDrv.c", el cual dispone de interrupciones y de buffers de
	software para no saturar el hardware propio del hardware, "zigbee.c", que se encarga
	de parsear la trama zigbee para su env�o o recepci�n, y de "bluetooth.c", que hace lo
	mismo pero para el bluetooth. Todos estos ficheros disponen de sus archivos de
	cabecera y aparte existe el "uartConfig.h" que sirve para definir la configuraci�n
	del driver de la UART.
	
	Los sensores de los que dispone la placa se encuentran dentro de la carpeta
	"sensores": "sensorEstado.c", "sensorObras.c", "sensorVelocidad.c" y
	"sensorVisibilidad.c". Por ahora estos sensores no est�n conectados a la placa por lo
	que se simulan mediante el keypad.
	
	Finalemente, la l�gica del programa se hace mediante una m�quina de estados. Esta
	m�quina tiene definido su motor en "motorAutomatas.c" y sus estados y transiciones se
	encuentran en "automata.c".
	