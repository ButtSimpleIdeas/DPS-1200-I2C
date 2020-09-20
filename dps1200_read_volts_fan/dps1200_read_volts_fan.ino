/*
  DPS1200_read_volts_fan

  Uses the Serial Mon to display the user status information from the Power Supply's PIC controller.
  
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

// Simple sketch to read power supply stats over I2C
// to arduino serialmon every 2 seconds.

uint8_t addy = 0x5f; //Supply Address verify w/I2C scanner
uint8_t i; // index for loop
uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e}; 

// Register Values 
// 0x08 = grid Voltage, 0x0a = grid Current, 0x0e = output voltage, 0x10 = output current
// 0x1c = internal temp, 0x1e = fan speed

uint8_t cs, regCS;
uint16_t msg[3]; //Received messages from PS
float ret, Stat; //reused calculated values

void setup() {Wire.begin(); Serial.begin(9600);}

void loop() {  

Serial.print ("addy = "); Serial.println (addy, HEX);
for (i = 0; i<6; i++) {

 cs = (addy<<1)+ reg[i];
 regCS = ((0xff - cs) +1 ) & 0xff; //calculate checksum of add+reg
  Wire.beginTransmission(addy); //open i2c with dps1200
  Wire.write(reg[i]); // dps1200 cycle through registers
  Wire.write(regCS); // write checksum each time
  Wire.endTransmission();  // close DPS1200 i2c
  delay(1); // Short delay between operations
 
  Wire.requestFrom(addy, 3);  
  msg[0] = Wire.read(); msg[1] = Wire.read();msg[2] = Wire.read();
  ret = (msg[1] << 8) + msg[0]; // Shift to MSB + LSB

if ( i == 0) { Stat = ret / 32 ; // scale factor
    Serial.print ("Grid Voltage = "); Serial.print (Stat);Serial.print ("V   ");}

if ( i == 1) { Stat = ret / 128 ; // scale factor
    Serial.print ("Grid Current = "); Serial.print (Stat);Serial.print ("A   ");}

if ( i == 2) { Stat = ret / 256; // scale factor
    Serial.print ("Output Voltage = "); Serial.print (Stat);Serial.print ("V   ");}

if ( i == 3) { Stat = ret / 128; // scale factor
    Serial.print ("Output Current = "); Serial.print (Stat);Serial.println ("A  ");}

if ( i == 4) { Stat = ret / 32; // scale factor
    Serial.print ("Internal Temp = "); Serial.print (Stat);Serial.print ("F  ");}
       
if ( i == 5) { Stat = ret ; // scale factor = 1
    Serial.print ("Fan RPM = "); Serial.print (Stat);Serial.print (" RPM   ");}
}
  Serial.println ("  "); Serial.println ("  ");delay (2000);
}
