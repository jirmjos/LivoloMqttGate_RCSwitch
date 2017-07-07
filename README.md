# RCSwitchMqttGate
Remote Control and MQTT Gateway of Livolo Switch & Other (RCSwitch library supported devices) 433Mhz devices


Gateway y control basado en ESP8266 y modulos transmisor y receptores 433,92 Mhz o transceptores.
Hace uso de la generacion interna de codigos de id remoto (testados hasta un numero de 30) y de los codigos de escenas I y II de Livolo lo que nos permite la gestion mediante comandos de ON y de OFF de forma separada y asi poder evitar usar los habitaules comandos unicos para cambio ON/OFF.
La programacion de los interruptores se hace de forma muy sencilla poniendo el interruptor en modo apredizaje o programacion (pulsandolo durante 5 segundos) y dejandolo en el estado de encendido o apagado que se requiera programar y a continuacion enviar el comando de encendido o apagado correspondiente al estado a programar del interruptor.
La forma de la orden MQTT topic/subtopic y comando es la siguiente:
livolo/switch1...30/0 o 1
Ejemplo para encendido del switch 1
l1volo/switch1/1
Topic: livolo
Subtopic: switch1 (1 a 30)
Comando:0 (OFF) o 1 (ON)


