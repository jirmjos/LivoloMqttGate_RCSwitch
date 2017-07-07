# RCSwitchMqttGate
Remote Control and MQTT Gateway of Livolo Switch & Other (RCSwitch library supported devices) 433Mhz devices


Gateway MQTT orientado al control RF de interruptores Livolo y tambien los soportados por la propia libreria RCSwitch basado en hardware de Expresiff ESP8266 y cualesquiera de los tipicos modulos OOK/ASK de transmisor y receptor de 433,92 Mhz (STX882/SRX802, RBX06, TA1001, etc...) o transceptores OOK de 433 (RFM69).
Hace uso de la generacion interna de codigos de id remoto (testados en la practica hasta un total de 30 codigos) y de los codigos de escenas I y II (code 10 y code 18 de room 1) de Livolo lo que nos permite la gestion mediante comandos de ON y de OFF de forma separada y asi poder evitar usar los habitaules comandos unicos para cambio ON/OFF.
La programacion de los interruptores se hace de forma muy sencilla poniendo el interruptor en modo apredizaje o programacion (pulsandolo durante 5 segundos) y dejandolo en el estado de encendido o apagado que se requiera programar y a continuacion enviar el comando de encendido o apagado correspondiente al estado a programar del interruptor.
La forma de la orden MQTT topic/subtopic y comando es la siguiente:
livolo/switch1...30/0 o 1
Ejemplo para encendido del switch 1
l1volo/switch1/1
Topic: livolo
Subtopic: switch1 (1 a 30)
Comando:0 (OFF) o 1 (ON)


