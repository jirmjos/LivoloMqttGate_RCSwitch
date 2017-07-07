#define DEBUG        // Descomentar para habilitar la depuracion con mensajes por el puerto serie
#define RCSW           // Para poder procesar dispositivos de radio 433Mhz compatibles con RCSWITCH
#define CLIENT_ID       "RCSWG1" // Nombre de nodo presentado en sevidor de MQTT
#define SUBSCR_TOPIC    "livolo/#" // Tema MQTT para interruptores
#define LIV_SW "livolo/switch%d" // Cadena MQTT para cada intrruptor (%d=1...n donde n es el numero total de interruptores)
#define LIV_PULT 2133   //Codigo idRemoto Livolo de inicio
#define sw_cnt          9 // Num. total de interruptores LIVOLO
#define send_rpt        3 // Num de reintentos de envio de señal de radio
#define TOPIC_TREE "433mhz/%Xh" //Plantilla del arbol MQTT
#define TOPIC_VAL "%Xh" // Valores para la plantilla del arbol MQTT
#define SIGNAL_IN 1     //Pin de conexion receptor radio 433Mhz 0 = DIGITAL PIN 2, 1 - PIN4
#define TRANS_OUT 4     //Pin de conexion transmisor radio 433Mhz
#define BUFFERSIZE 50   // Buffer de recepcion de mensajes radio
#define WAITAFTERSIGNAL 500 // Tiempo de espera para siguiente procesamiento al recibir nueva señal radio
// Codigos LIVOLO
#define CODE_ON 10      //Escena 1 (la asociaremos al estado de encendido del interruptor)
#define CODE_OFF 18     //Escena 1 (la asociaremos al estado de apagado del interruptor)
#define UPDATE_STATE   //Actualizar el estado de todas las unidades

#include "WIFI_and_broker_parameters.h" //parametros especificos para conexion a wi-fi y broker MQTT

//#include <UIPEthernet.h> //Si usamos ethernet en arduino
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "livolo.h"
WiFiClient wifiClient;
//EthernetClient ethClient; /Si usamos ethernet en arduino

#ifdef RCSW
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
#endif

char topic[16];
char msg[8];

// connect transiver and create class livolo
Livolo livolo(TRANS_OUT); 

volatile byte impulse = 0; 
volatile int bufor[BUFFERSIZE];
volatile boolean header = false;
volatile unsigned long StartPeriod = 0; 
volatile boolean stop_ints = false;

volatile int i;
volatile byte state[sw_cnt]; 

const uint8_t mac[6] = {0x13,0x12,0x19,0x77,0x00,0x01};
const byte server[] = { 192, 168, 1, 10 }; // mqtt server
volatile long previousMillis;

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  byte st = 0;
  char t[16];
  st = atoi((char*)payload);
  for (i=0;i<sw_cnt;i++) {
    sprintf(t, LIV_SW, i+1);
    if (strcmp (t,topic) == 0) {
      if (state[i]!=st) {
        state[i]=st;
        // Radio send
        int remote = LIV_PULT+i;
        int code = CODE_OFF;
        if (state[i]==1) { code=CODE_ON; }
        #ifdef DEBUG
        Serial.println('Send LIVOLO:');
        Serial.println(remote);
        Serial.println(code);
        #endif
        noInterrupts();
        for (i=0;i<send_rpt;i++) {
         livolo.sendButton(remote,code); 
         }
        interrupts();
        break;
       }
     }
  }
}

PubSubClient mqttClient(server, 1883, callback, wifiClient);

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("Livolo Switchs Handler v1.0 @jirm - 2017"));
  #endif
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    //if(Ethernet.begin(mac) == 0) {
     #ifdef DEBUG
     Serial.print("Connecting to WIFI");
     #endif
     while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(1000);
    }
    #ifdef DEBUG
    Serial.println("Connected to WIFI!");
    #endif 
    for(;;);

 #ifdef UPDATE_STATES
  for (i=0;i<sw_cnt;i++) {
    state[i]=2;
  }
 #endif
 
 previousMillis = millis();
 attachInterrupt(SIGNAL_IN, calcInput, CHANGE); 
}

  
void calcInput()
{
  #ifdef RCSW
    mySwitch.handleInterrupt();
  #endif
  
  // get the time using micros
  unsigned int duration = (int)(micros() - StartPeriod); // save pulse length to bufor
  StartPeriod = micros(); //begin next impulse
 
  if (stop_ints) return;
  if ((duration < 90) || (duration > 600)) {
  //  Serial.println(duration);
    goto reset; //impulse not right
  }
  bufor[impulse++] = duration;
  if (duration < 415) return;
  if (!header)
  {
    header = true;
    impulse = 0;
    return;
  }
  else
  {
    if ((impulse < 23) || (impulse > (BUFFERSIZE-1))) goto reset; //too long or too short info
    stop_ints = true;
    return;
  }
reset:
  header = false;
  impulse = 0;
  return;
}


void reconnect(){

  while(!mqttClient.connected()) {
   #ifdef DEBUG
   Serial.println(F("Connect MQTT"));
   #endif
   if(mqttClient.connect(CLIENT_ID)) {
     mqttClient.subscribe(SUBSCR_TOPIC); 
    } else {
     #ifdef DEBUG
     Serial.println(F("Error Connection MQTT"));
     #endif
    } 
   }
}


void recieve()
{
#ifdef RCSW
   if (mySwitch.available()) {
     int value = mySwitch.getReceivedValue();
     mySwitch.resetAvailable();
     sprintf(topic, TOPIC_TREE, value);
     sprintf(msg, TOPIC_VAL, micros());
      #ifdef DEBUG
        Serial.println('Send to MQTT:');
        Serial.println(topic);
        Serial.println(msg);
      #endif
      reconnect();
      mqttClient.publish(topic,msg);
      previousMillis = millis();
    }
#endif
  
  if (stop_ints) //data in buffer
  {
    unsigned long binary = 1;
    //byte i = 0;
    for (byte j = 0; j < BUFFERSIZE-1; j++)
    {
      if ((bufor[j] > 220) &&
          (bufor[j] < 400))
      {
        binary <<= 1;
        //binary |= 1;
        //i++;
        bitSet(binary,0);
      }
      else if ((bufor[j] > 90) &&
               (bufor[j] < 220) && (bufor[j + 1] > 90) &&
               (bufor[j + 1] < 220)) {
        binary <<= 1;
        j++;
        
      }
      else if ((bufor[j] > 90) &&
               (bufor[j] < 220) && (bufor[j + 1] > 220) &&
               (bufor[j + 1] < 400)) {
        binary <<= 1;
        bitSet(binary,0);
        //i += 2;
        j++;
      }
      else break;
    }

    if (bitRead(binary,23))
    {
      bitClear(binary,23);
    }
      previousMillis=micros();
      sprintf(topic, TOPIC_TREE, binary);
      sprintf(msg, TOPIC_VAL, previousMillis);
      #ifdef DEBUG
      Serial.println('Send to MQTT:');
      Serial.println(topic);
      Serial.println(msg);
      #endif
      reconnect();
      mqttClient.publish(topic,msg);
      previousMillis = millis();
      header = false;
      impulse = 0;
      stop_ints = false;

  }
}

void loop() {
  
  if ((millis() - previousMillis)>WAITAFTERSIGNAL) {
     recieve();
  }
  reconnect();
  mqttClient.loop();
}

