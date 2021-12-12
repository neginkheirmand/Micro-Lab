#include <Wire.h>

#define ADDR_Ax 0b000 //A2, A1, A0
#define ADDR (0b1010 << 3) + ADDR_Ax //creating the address of the slave eeprom (the signals that define which slave and in what type of communication R/W)


void setup() {
  Serial.begin(9600);
  Wire.begin();
  //the default
  writeI2CByte(0, 4);
  for (byte i = 1; i <=4 ; i++) {
    writeI2CByte(i, i);
  }
  //the custom (by default also has the value of 1-2-3-4)
  writeI2CByte(6, 4);
  for (byte i = 1; i <=4 ; i++) {
    writeI2CByte(i+6, i);
  }
  
  for (byte i = 0; i <= 20; i++) {
    byte b = readI2CByte(i);
    String a = "";
    a = a + String(i);
    a = a + "  ";
    a = a + String(b);
    Serial.println(a);
  }

  //just for the machine to work properly the values for the indexOfWork and default will also be set properly
  writeI2CByte(17, 0);//indexOfWork
  readI2CByte(18, 1);//flagDefault
}

void loop() {
   
}

void writeI2CByte(byte data_addr, byte data){
  Wire.beginTransmission(ADDR);
  delay(10);
  Wire.write(data_addr);
  delay(10);
  Wire.write(data);
  delay(10);
  Wire.endTransmission();
}

byte readI2CByte(byte data_addr){
  byte data = NULL;
  Wire.beginTransmission(ADDR);
  delay(10);
  Wire.write(data_addr);
  delay(10);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(ADDR, 1); //retrieve 1 returned byte
  delay(10);
  if(Wire.available()){
    data = Wire.read();
    delay(10);
  }
  return data;
}
