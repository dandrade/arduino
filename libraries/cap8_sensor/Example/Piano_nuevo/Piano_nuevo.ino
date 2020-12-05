#include <Wire.h> //Biblioteca I2C
#include <cap8_sensor.h>  //Biblioteca para el sensor de la inkcu Shield

cap8_sensor cap = cap8_sensor(); 
 int notes[] = {1046,988,880,784,698,659,587,523};
//int Buzzer = 5, speaker 3;
int speaker = 5;
 int note_duration = 100;
// int buttonInc = 6;
// int buttonDec = 7;
// int index;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("CAP1298 test!");

  // Inicializa el sensor enviando la dirección I2C
   if (!cap.begin(0x28)) {
    Serial.println("CAP1298 not found");
    while (1);
  }
  Serial.println("CAP1298 found!");

  cap.writeRegister(0x2A, 0x80); // Turn off multitouch
  cap.writeRegister(0x41, 0x39);
  cap.writeRegister(0x1F, 0x5F); //Sensitivity 4x

  pinMode(speaker, OUTPUT);
//  pinMode(buttonInc, INPUT);
//  pinMode(buttonDec, INPUT);

  //index = 5;  Indice de sensibilidad
    
}

void loop() {
  // put your main code here, to run repeatedly:

  uint8_t touched = cap.touched();

/***Rutina para aumentar y disminuir la sensibilidad**/
//  if(digitalRead(buttonInc) == LOW && index < 8){
//      index++;
//      
//    }else if(digitalRead(buttonDec) == LOW && index >= 0){
//        index--;
//      }
//  delay(50);

//  setSensitivity(index);
/*****************************************************/

  switch(touched){
      case 0: break;
      case 1: tone(speaker, notes[0], note_duration);break;
      case 2: tone(speaker, notes[1], note_duration);break;
      case 4: tone(speaker, notes[2],note_duration);break;
      case 8: tone(speaker, notes[3],note_duration);break;
      case 16: tone(speaker, notes[4],note_duration);break;
      case 32: tone(speaker, notes[5],note_duration);break;
      case 64: tone(speaker, notes[6],note_duration);break;
      case 128: tone(speaker, notes[7],note_duration);break;
      default: break;
    }

 Serial.println(touched);

  delay(50);  

}

/***Función para poner la sensibilidad***/

//void setSensitivity(int i){
//           
//
//  switch(i){
//      case 0: cap.writeRegister(0x1F, 0x7F); Serial.println("Sens: 1x");break;
//      case 1: cap.writeRegister(0x1F, 0x6F); Serial.println("Sens: 2x"); break;
//      case 2: cap.writeRegister(0x1F, 0x5F); Serial.println("Sens: 4x"); break;
//      case 3: cap.writeRegister(0x1F, 0x4F); Serial.println("Sens: 8x"); break;
//      case 4: cap.writeRegister(0x1F, 0x3F); Serial.println("Sens: 16x"); break;
//      case 5: cap.writeRegister(0x1F, 0x2F); Serial.println("Sens: 32x"); break;
//      case 6: cap.writeRegister(0x1F, 0x1F); Serial.println("Sens: 64x"); break;
//      case 7: cap.writeRegister(0x1F, 0x0F); Serial.println("Sens: 128x"); break;   
//      default: break;
//    }
//  }



