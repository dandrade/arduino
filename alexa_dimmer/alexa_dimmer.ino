#include "hw_timer.h" 

 /*
 * DIMMER AZUL 2 CANALES
 GND - GND
 VCC - 5V
 PD2 - D7
 PZC - D6
*/
#include <ESP8266WiFi.h>
//#include "FirebaseArduino.h"

//#define FIREBASE_HOST "casa1-fefaa.firebaseio.com" // conecxion firebase
//#define FIREBASE_AUTH "8oOpxghij3kRZxOwxzgSsFkAs0ksHhbxkraLZlPR"
//------------------------------

// wifi credenciales
#define WIFI_SSID "Noble"
#define WIFI_PASSWORD "sacalasperlas"

//#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  
  #define RELAY_PIN_1 12
  #define RELAY_PIN_2 14
#else
  #include <ESP8266WiFi.h>
  
  #define RELAY_PIN_1 4
  #define RELAY_PIN_2 14
#endif
#include "fauxmoESP.h"

//#include <RCSwitch.h>

#define SERIAL_BAUDRATE 115200

#define LAMP_1 "luz"

fauxmoESP fauxmo;

int anterior = 999;
int dimmer;
int val;
int resetBot = 14; // este pin manda el pulso al pin RST (PARA HACER RESET)
 
const byte zcPin = 12; // D6
const byte pwmPin = 13; // D7

byte fade = 0;
byte state = 1;
byte tarBrightness = 255;
byte curBrightness = 0;
byte zcState = 0; // 0 = ready; 1 = processing;
void setup() {
  Serial.begin(9600);
 // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("connected ");
  Serial.println(WiFi.localIP());
  //Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  delay(1000);

  //----------
     
  pinMode(zcPin, INPUT_PULLUP);
  pinMode(pwmPin, OUTPUT);
  attachInterrupt(zcPin, zcDetectISR, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  hw_timer_init(NMI_SOURCE, 0);
  hw_timer_set_func(dimTimerISR);

  // By default, fauxmoESP creates it's own webserver on the defined port
  // The TCP port must be 80 for gen3 devices (default is 1901)
  // This has to be done before the call to enable()
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
  fauxmo.enable(true);
  // You can use different ways to invoke alexa to modify the devices state:
  // "Alexa, turn lamp two on"

  // Add virtual devices
  fauxmo.addDevice(LAMP_1);
  //fauxmo.addDevice(LAMP_2);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Callback when a command from Alexa is received. 
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.
        
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    Serial.println(state);
    if ( (strcmp(device_name, LAMP_1) == 0) ) {
      // this just sets a variable that the main loop() does something about
      Serial.println("RELAY 1 switched by Alexa");
      //digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      dimmer = value;
      if(dimmer != anterior){
        Serial.print("envia al arduino: ");
        Serial.println(dimmer);
        val = dimmer;
        tarBrightness =val;
        Serial.println(dimmer);
        anterior=dimmer;
        digitalWrite(pwmPin, dimmer);
      }else{
       // no manda nada
      }
      /*if (state) {
        
        digitalWrite(pwmPin, dimmer);
        //Firebase.setFloat("/casa1/dimmer", value); 
      } else {
        digitalWrite(pwmPin, dimmer);
        //Firebase.setFloat("/casa1/dimmer", value); 
      }*/f
    }
    /*if ( (strcmp(device_name, LAMP_2) == 0) ) {
      // this just sets a variable that the main loop() does something about
      Serial.println("RELAY 2 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_2, LOW);
      } else {
        digitalWrite(RELAY_PIN_2, HIGH);
      }
    }*/
  });

 
}

void loop() {

    // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

     /*dimmer = Firebase.getFloat("/casa1/dimmer"); 
     
     if(dimmer != anterior){
        Serial.print("envia al arduino: ");
        Serial.println(dimmer);
        val = dimmer;
        tarBrightness =val;
        Serial.println(dimmer);
        anterior=dimmer;
        digitalWrite(pwmPin, dimmer);
      }else{
       // no manda nada
      }*/


    /*if (Firebase.failed()) {
        
        Serial.print("Error en connexion de firebase:");
        Serial.println(Firebase.error());
        delay(1000);
     // el super reset (si hay error en firebase hace un reset)
           pinMode(resetBot, OUTPUT);
           delay(2000);
           pinMode(resetBot, INPUT);
     //end super reset
       // return;    
  }*/

}


void dimTimerISR() {
    if (fade == 1) {
      if (curBrightness > tarBrightness || (state == 0 && curBrightness > 0)) {
        --curBrightness;
      }
      else if (curBrightness < tarBrightness && state == 1 && curBrightness < 255) {
        ++curBrightness;
      }
    }
    else {
      if (state == 1) {
        curBrightness = tarBrightness;
      }
      else {
        curBrightness = 0;
      }
    }
    
    if (curBrightness == 0) {
      state = 0;
      digitalWrite(pwmPin, 0);
    }
    else if (curBrightness == 255) {
      state = 1;
      digitalWrite(pwmPin, 1);
    }
    else {
      digitalWrite(pwmPin, 1);
    }
    
    zcState = 0;
}

void zcDetectISR() {
  if (zcState == 0) {
    zcState = 1;
  
    if (curBrightness < 255 && curBrightness > 0) {
      digitalWrite(pwmPin, 0);
      
      int dimDelay = 30 * (255 - curBrightness) + 400;//400
       hw_timer_arm(dimDelay);
    }
  }
}
