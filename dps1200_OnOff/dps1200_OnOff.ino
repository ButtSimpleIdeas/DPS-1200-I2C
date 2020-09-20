/*
  DPS1200_OnOff

  Uses the Serial Mon to query the user to turn ON or turn OFF the Power Supply.
  
  The R_On of the I2C adapter is connected to D7 on the Arduino Uno.
  Ground of the adapter header is connected to ground of the Arduino.
  
  Modified 09.20.2020 by Garry Mercaldi
  Butt Simple Ideas, LLC
  
  This example code is in the public domain.

*/
char Selection = 0; //Global Select variable

void setup() {
 pinMode(7, OUTPUT);
 Serial.begin(9600); // Sets the Serial Mon Baud rate
}

void loop() {
 Serial.println ("Make Selection 1=ON, 2=OFF"); 
 while (Serial.available() == 0) { }; // Wait for Serial Mon input
 
   Selection = Serial.read(); Serial.print ("You Selected: "); Serial.print (Selection); 
   Serial.print ("   "); 
   if(Selection == '1') { Serial.println ("Supply On ");digitalWrite(7, HIGH); };    
   if(Selection == '2'){ Serial.println("Supply Off"); digitalWrite(7, LOW);}
   Serial.println ("   "); Serial.println ("   ");
}
