/*
cap8_sensor.h  - Library to communicate with the CAP1298
Autor: Tomás Santiago

*/

#ifndef cap_sensor_h
#define cap_sensor_h

#if (ARDUINO < 100)
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

#include <Wire.h>

// SMBus Address
#define CAP1298_ADDR	0x28


//Registers
#define MAIN_CONTROL		0x00
#define MAIN_INT			0x01
// #define GENERAL_STATUS		0x02
#define INPUT_STATUS		0x03
/*
#define	NOISE_FLAG_ST		0x0A
#define	SENSOR1_DELTA		0x10
#define	SENSOR2_DELTA		0x11
#define	SENSOR3_DELTA		0x12
#define	SENSOR4_DELTA		0x13
#define	SENSOR5_DELTA		0x14
#define	SENSOR6_DELTA		0x15
#define	SENSOR7_DELTA		0x16
#define	SENSOR8_DELTA		0x17

*/
#define	SENSITIVITY_CONTROL	0x1F
/*#define	CONFIGURATION		0x20
#define	SENSOR_INPUT_EN		0x21
#define	SENSOR_INPUT_CONF	0x22
#define	SENSOR_INPUT_CONF2	0x23
#define	AV_SAMPLING_CONF	0x24
#define	CAL_ACT_AND_STAT	0X26	
#define	INTERRUPT_EN		0x27
#define	REPEAT_RATE_EN		0x28
#define SIGNAL_GUARD_EN		0x29
*/
#define	MULT_TOUCH_CONF		0x2A
/*#define	MULT_TOUCH_PATT		0x2B
#define	BASE_COUNT_OUT		0x2E
#define	RECAL_CONF			0x2F
#define SENSOR1_INPUT_THR	0x30
#define SENSOR2_INPUT_THR	0x31
#define SENSOR3_INPUT_THR	0x32
#define SENSOR4_INPUT_THR	0x33
#define SENSOR5_INPUT_THR	0x34
#define SENSOR6_INPUT_THR	0x35
#define SENSOR7_INPUT_THR	0x36
#define SENSOR8_INPUT_THR	0x37
#define	SENSOR_NOISE_THR	0x38
#define	STANDBY_CHANNEL		0x40
*/
#define	STANDBY_CONF		0x41
#define	PRODUCT_ID			0xFD
#define MANUFACTURER_ID		0xFE
#define	REVISION			0XFF


class cap8_sensor
{
public:
	cap8_sensor();
	
	boolean begin();
	//void setSensitivity(uint8_t value);
  	uint8_t readRegister(uint8_t reg);
  	void writeRegister(uint8_t reg, uint8_t value);
	void setSensitivity(uint8_t value);
	void multitouch(bool value);
  	uint8_t touched(void);
	uint8_t readdelta(uint8_t sensor);
	void defaultthreshold(void);
	void threshold(uint8_t sensor, uint8_t value);
	void setpattern(bool set,uint8_t perc); //perc 0->3%25
	void sensorsonpattern(bool c1,bool c2,bool c3,bool c4,bool c5,bool c6,bool c7,bool c8);

private:
	 int8_t _addr;
};


#endif