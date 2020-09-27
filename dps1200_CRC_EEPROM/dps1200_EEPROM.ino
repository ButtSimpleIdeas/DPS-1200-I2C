/*
  DPS1200_EEPROM
  Uses the Serial Mon to create a menu to read or write to the EEPROM, and to calculate CRC.
  
  The Adapters I2C header is connected to the Arduino Uno as follows:
  Ground - Ground
  +5V - +5V
  I2C Header SDA - Arduino A4
  I2C Header SCL - Arduino A5.
  
  Modified 09.23.2020 by Garry Mercaldi
  Butt Simple Ideas, LLC
  
  This example code is in the public domain.

*/
#include <Wire.h>
#define DPSdevice 0x5f //DPS-1200 PIC address

byte data, wordaddress;
byte index, index1, buffer[16];
byte count, select;
byte result;
int data2;
uint8_t len = 2;
byte data1[] = { 0x5f, 0x40 }; // **** make this a selection

void setup() {
 Wire.begin();
 Serial.begin(9600); // 115200
 delay(500);
}//end setup


void loop() {

  char inByte = 0;
  Serial.println(" "); Serial.println(" ");
  Serial.println("Find Addresses = 1");
  Serial.println("Calculate CRC-8 Byte= 2");  
  Serial.println("DumpEEPROM = 3");
  Serial.println("Write EEPROM Byte= 4");
  Serial.println(" ");  Serial.println(" ");


  while (Serial.available() == 0) { };
    inByte = Serial.read(); Serial.println (inByte); 
    
   if(inByte == '1') { Serial.println ("\nFind I2C Address "); FindI2C (); };    
   if(inByte == '2')  { Serial.println("\nCalculate new CRC-8"); byte crc = Compute_CRC8(data1, sizeof(data1));}
   if(inByte == '3'){ Serial.println("\nDump 24C02 EEPROM"); DumpEEPROM();};
   if(inByte == '4') { Serial.println("\nWrite 24C02 EEPROM Byte"); WriteEEPROM();};  
  };

void showReadAsciiBlock(byte data, byte wordaddress){
  if (data < 32) {Serial.print('.');}          // do not print control codes
   else if (data ==92) {Serial.print('.');}    // do not print "\"
   else if (data >126) {Serial.print('.');}    // do not print 'extended' characters
   else Serial.write (data);
}

void deviceWrite(byte data, byte wordaddress) {
 Wire.beginTransmission(DPSdevice);
 Wire.write(wordaddress);
 Wire.write(data);
 Wire.endTransmission();
}

int deviceRead(byte wordaddress) {
 Wire.beginTransmission(DPSdevice);
 Wire.write(wordaddress);
 Wire.requestFrom(DPSdevice, 1);
 if(Wire.available()) {
   data2 = Wire.read();
 }
 Wire.endTransmission();
 return data2;
}


void DumpEEPROM () {
 data = 0x01;
 wordaddress = 0x00;
 for(index=0x00;index<=15;index++){
     Serial.print(wordaddress,HEX); 
     Serial.print("\t");
     for(index1=0x00;index1<=15;index1++){
       buffer[index1] = deviceRead(wordaddress);
       showReadAsciiBlock(buffer[index1],wordaddress); //Print ASCII
       delay(10);
       wordaddress++;
    };
      Serial.print("\t"); //Print Tab
      Serial.print("| "); //Print beginning Seperator
      for(index1=0x00;index1<=15;index1++){
        if (buffer[index1] <= 15) { Serial.print("0"); }; // Padding
        Serial.print (buffer[index1], HEX); Serial.print(" ");//Print HEX
      };
     Serial.print(" |"); Serial.println(" "); // //Print end Seperator and next line
   }; 
  };
 
void FindI2C () {
  byte errorResult;           // error code returned by I2C Wire.endTransmission()
  byte i2c_addr;              // I2C address being pinged
  byte lowerAddress = 0x08;   // I2C lowest valid address in range
  byte upperAddress = 0x77;   // I2C highest valid address in range
  byte numDevices;            // how many devices were located on I2C bus

  Serial.print("Scanning I2C 7-bit address range 0x");
  if (lowerAddress < 0x10) // pad single digit addresses with a leading "0"
  Serial.print("0"); Serial.print(lowerAddress, HEX);
  Serial.print(" to 0x"); Serial.print(upperAddress, HEX);Serial.println(".");
  numDevices = 0;
  for (i2c_addr = lowerAddress; i2c_addr <= upperAddress; i2c_addr++ )    
  {
    Wire.beginTransmission(i2c_addr);                  // initiate communication at current address
    errorResult = Wire.endTransmission();              // if a device is present, it will send an ack and "0" will be returned from function
    if (errorResult == 0)                              // "0" means a device at current address has acknowledged the serial communication
    {
      Serial.print("I2C device found at address 0x");
      if (i2c_addr < 0x10)                             // pad single digit addresses with a leading "0"
        Serial.print("0");
        Serial.println(i2c_addr, HEX);                   // display the address on the serial monitor when a device is found
        numDevices++;
    }
  }
  Serial.print("Scan complete.  Devices found: ");
  Serial.println(numDevices);
  Serial.println();
}

  void WriteEEPROM () {
    Serial.println("Enter address to Write (0-255): ");  
    while (Serial.available() == 0)   
    {  byte wordaddress = Serial.read(); };//Reading the Input string from Serial port.  
    data = deviceRead(wordaddress);
    Serial.print("Original read value was: "); Serial.println(data, HEX);  
    Serial.print("Writing Data ");    
    deviceWrite(data, wordaddress);
    delay (100);
    data = deviceRead(wordaddress);
    Serial.print("Post write value is: "); Serial.println(data, HEX);  
};

byte Compute_CRC8(byte *bytes, int len) { 
  // The CRC-8 polynomial used in SMBus revision 1.1 is  
  // 8bit-CRC: 0x07 =  x8  + x2  + x + 1  C(x) = 1 0000 0111.    
  byte crc = 0;
  while (len--)
  {
    crc ^= *bytes++; /* XOR-in the next input byte */
    for (int i = 0; i < 8; i++)
    { if ((crc & 0x80) != 0) { crc = (byte)((crc << 1) ^ B00000111); }
      // polynomial = 0x07  =  x8  + x2  + x + 1 (ignore MSB which is always 1)
      else { crc <<= 1; }
    }
  }
//  Serial.println(data1[0], HEX); Serial.println(data1[1], HEX);
//  Serial.print("CRC = "); Serial.println(crc, HEX);
  return crc;
}
