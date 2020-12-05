/*
cap8_sensor.cpp
*/

#include "cap8_sensor.h"

cap8_sensor::cap8_sensor(){
}


boolean cap8_sensor::begin(){
	_addr=0x28;
	Wire.begin();
	 readRegister(PRODUCT_ID);
  
  // Useful debugging info
  
  Serial.print("Product ID: 0x");
  Serial.println(readRegister(PRODUCT_ID), HEX);
  Serial.print("Manuf. ID: 0x");
  Serial.println(readRegister(MANUFACTURER_ID), HEX);
  Serial.print("Revision: 0x");
  Serial.println(readRegister(REVISION), HEX);
  

  if ( (readRegister(PRODUCT_ID) != 0x71) ||
       (readRegister(MANUFACTURER_ID) != 0x5D) ||
       (readRegister(REVISION) != 0x01)) {
    return false;
  }
  // allow multiple touches
  writeRegister(MULT_TOUCH_CONF, 0); 
  // speed up a bit
  writeRegister(STANDBY_CONF, 0x30);
  return true;
}


uint8_t  cap8_sensor::touched(void) {
  uint8_t st = readRegister(INPUT_STATUS);
  if (st) {
    writeRegister(MAIN_CONTROL, readRegister(MAIN_CONTROL) & ~MAIN_INT);
  }
	return st;
}

static uint8_t i2cread(void) {
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}

static void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}

uint8_t cap8_sensor::readRegister(uint8_t reg){
	Wire.beginTransmission(_addr);
    i2cwrite(reg);
    Wire.endTransmission();
    Wire.requestFrom(_addr, 1);
    return (i2cread());
}

void cap8_sensor::writeRegister(uint8_t reg, uint8_t value){
	Wire.beginTransmission(_addr);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value));
    Wire.endTransmission();
}
void cap8_sensor::setSensitivity(uint8_t value){//0-7niveles 
if (value>=0&&value<8){
value = 7-value;
value = value <<4;
value = value | 0x0F;
}
else{value= 0x8F;}
Wire.beginTransmission(_addr);
i2cwrite((uint8_t)0x1F);
i2cwrite((uint8_t)(value));
Wire.endTransmission();
}

void cap8_sensor::multitouch(bool value){//set multitouch

if (value==false){
Wire.beginTransmission(0x28);
    i2cwrite((uint8_t)0x2A);
    i2cwrite((uint8_t)(0x80));
    Wire.endTransmission();
}
else{
Wire.beginTransmission(0x28);
    i2cwrite((uint8_t)0x2A);
    i2cwrite((uint8_t)(0x00));
    Wire.endTransmission();

}
}

uint8_t cap8_sensor::readdelta(uint8_t sensor){//lee delta 0->7c
if(sensor<14){
sensor = sensor | 0x10;
    Wire.beginTransmission(_addr);
    i2cwrite(sensor);
    Wire.endTransmission();
    Wire.requestFrom(_addr, 1);
    return (i2cread());
}
else{
return 0;
}
}

void cap8_sensor::defaultthreshold(void){//defaultthreshol
int addr = 0x30;
for (int i=0;i<8;i++)
{
    Wire.beginTransmission(0x28);
    i2cwrite((uint8_t)addr);
    i2cwrite((uint8_t)(0x40));
    Wire.endTransmission();
    addr++;
}
}

void cap8_sensor::threshold(uint8_t sensor, uint8_t value){
//(0-7sensor cap,threshold value)
if (sensor>=0&&sensor<8){
sensor = 0x30|sensor;
	if (value>=0&&value<128){
		Wire.beginTransmission(_addr);
		i2cwrite((uint8_t)sensor);
		i2cwrite((uint8_t)(value));
		Wire.endTransmission();
	}
}
}

void cap8_sensor::setpattern(bool set,uint8_t perc){
//0->12.5/1->25/2->37.5/4->100
uint8_t value;
value = 0x83; //configuración para activar patron 1000 0011
if(set){
	if(perc<=0&&perc<4)
	{
		perc = perc<<2;
		value= perc|value;
	}
}
else{
	value=0x00;
}
		Wire.beginTransmission(_addr);
		i2cwrite((uint8_t)0x2B);
		i2cwrite((uint8_t)(value));
		Wire.endTransmission();

}

void cap8_sensor::sensorsonpattern(bool c1,bool c2,bool c3,bool c4,bool c5,bool c6,bool c7,bool c8){
uint8_t a1,a2,a3,a4,a5,a6,a7,a8,value;
if(c1){a1=1;}else{a1=0;}
if(c2){a2=2;}else{a1=0;}
if(c3){a3=4;}else{a1=0;}
if(c4){a4=8;}else{a1=0;}
if(c5){a5=16;}else{a1=0;}
if(c6){a6=32;}else{a1=0;}
if(c7){a7=64;}else{a1=0;}
if(c8){a8=128;}else{a1=0;}
value = a1+a2+a3+a4+a5+a6+a7+a8;
Wire.beginTransmission(_addr);
i2cwrite((uint8_t)0x2D);
i2cwrite((uint8_t)(value));
Wire.endTransmission();

}

