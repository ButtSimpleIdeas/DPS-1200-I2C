/*
  DPS1200_write_fan

  Uses the Serial Mon to allow the user to set the Power Supply's fan speed register PIC controller.
  Temporarily set fan speed to max, and Loops every 5 seconds
  Unplug supply to reset dps1200 to PIC control of the fan

  The Adapters I2C header is connected to the Arduino Uno as follows:
  Ground - Ground
  +5V - +5V
  I2C Header SDA - Arduino A4
  I2C Header SCL - Arduino A5.
  
  Modified 09.20.2020 by Garry Mercaldi
  Butt Simple Ideas, LLC
  
  This example code is in the public domain.

*/

#include <Wire.h>

uint8_t addy = 0x5f; //Supply Address verified w/12C scanner
uint8_t reg = 0x40;//fan speed reg
uint8_t valLSB = 0x00, valMSB = 0x00;
uint8_t cs = 0x00, regCS = 0x00;
uint16_t value = 0x1000; //full fan speed

void setup() {
Wire.begin();
Serial.begin(9600);
}

void loop() {  
  
  valLSB = value & 0xff; //strip lsb
  valMSB = value >> 8; //strip msb
  cs = (addy<<1)+ reg + valLSB + valMSB;
  regCS = ((0xff - cs) +1 ) & 0xff; //calculate checksum of message
 
  // ** Sets dps1200 fan speed to 0x1000
  Wire.beginTransmission(addy); //open i2c with dps1200
  Wire.write(reg); // dps1200 0x40 is fan base register
  Wire.write(valLSB); // write lsb
  Wire.write(valMSB); // write msb
  Wire.write(regCS); // write checksum 
  Wire.endTransmission();  // close dps1200 i2c

  // ** Prints values of routine to serialmon
  Serial.print ("addy = "); Serial.println (addy, HEX);
  Serial.print ("reg = "); Serial.println (reg, HEX);
  Serial.print ("value = "); Serial.println (value, HEX);
  Serial.print ("valLSB = "); Serial.println (valLSB, HEX);
  Serial.print ("valMSB = "); Serial.println (valMSB, HEX);
  Serial.print ("cs = "); Serial.println (cs, HEX);
  Serial.print ("regCS = "); Serial.println (regCS, HEX);
  Serial.println ("  "); Serial.println ("  ");
  delay (5000);
}
