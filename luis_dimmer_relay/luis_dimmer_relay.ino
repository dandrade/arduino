/*
 * Rui Santos 
 * Complete Project Details http://randomnerdtutorials.com
*/

#include <Arduino.h>
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

#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "domotica-cc7a7-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "e83ZvD4NQkAhy46582puABkFJVypd0tSIZs7YW3l"
#define WIFI_SSID "SalaTv"
#define WIFI_PASS "n5g5CfMM"

//#include <RCSwitch.h>

#define SERIAL_BAUDRATE 115200

#define LAMP_1 "pasillo"
#define FIRE_TAG "/senderos/pasillo" 

#define LAMP_2 "exterior1"
#define FIRE_TAG2 "/senderos/exterior1" 

fauxmoESP fauxmo;

const byte zcPin = 12; // D6
const byte pwmPin = 13; // D7

const int relay = 5; // D1
#include "hw_timer.h";

byte fade = 1;
byte state = 1;
byte tarBrightness = 255;
byte curBrightness = 0;
byte zcState = 0; // 0 = ready; 1 = processing;
int anterior = 999;
int dimmer;
int val;
//RCSwitch mySwitch = RCSwitch();

// Wi-Fi Connection
void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  // Wi-Fi connection
  wifiSetup();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // LED
  /*pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, HIGH);

  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_2, HIGH);
*/
  pinMode(zcPin, INPUT_PULLUP);
  pinMode(pwmPin, OUTPUT);
  pinMode(relay, OUTPUT);
  

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
  fauxmo.addDevice(LAMP_2);
  
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
      // get value 
      Serial.print("dimmer: ");
      float dimmer = Firebase.getFloat(FIRE_TAG);
      Serial.println(dimmer);
      delay(1000);

      // set value
      Firebase.setFloat(FIRE_TAG, value);
      // handle error
      if (Firebase.failed()) {
        Serial.print("setting /senderos/dimmer failed:");
        Serial.println(Firebase.error());  
        return;
      }
      delay(1000);

      if (state) {
        digitalWrite(pwmPin, value);
      } else {
        digitalWrite(pwmPin, value);
      }
    }

        if ( (strcmp(device_name, LAMP_2) == 0) ) {
      // this just sets a variable that the main loop() does something about
      Serial.println("RELAY 2 switched by Alexa");
      //digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      // get value 
      Serial.print("dimmer: ");
      bool relayF2 = Firebase.getBool(FIRE_TAG2);
      Serial.println(relayF2);
      delay(1000);

      // set value
      Firebase.setBool(FIRE_TAG2, value);
      // handle error
      if (Firebase.failed()) {
        Serial.print("setting /senderos/dimmer failed:");
        Serial.println(Firebase.error());  
        return;
      }
      delay(1000);

      if (state) {
        //digitalWrite(pwmPin, value);
      } else {
        //digitalWrite(pwmPin, value);
      }
    }
  
  });
  attachInterrupt(zcPin, zcDetectISR, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
 
  hw_timer_init(NMI_SOURCE, 0);
  hw_timer_set_func(dimTimerISR);
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

  dimmer = Firebase.getFloat(FIRE_TAG);
  if(dimmer != anterior){
    Serial.print("Seteando valor");
    Serial.println(dimmer);
    val = dimmer;
    tarBrightness = val;
    anterior = dimmer;
    digitalWrite(pwmPin, dimmer);
  }

  bool relayF = Firebase.getBool(FIRE_TAG2);
  
  if(!relayF){
    Serial.println("Seteando traseras valor HIGH");
    Serial.println(relayF);
    digitalWrite(relay, HIGH);
  }
  else {
    Serial.println("Seteando traseras valor LOW");
    Serial.println(relayF);
    
    digitalWrite(relay, LOW);
   }

  // get value 
 /* Serial.print("dimmer: ");
  Serial.println(Firebase.getFloat("/senderos/dimmer"));
  delay(1000);
   */ 
  
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
