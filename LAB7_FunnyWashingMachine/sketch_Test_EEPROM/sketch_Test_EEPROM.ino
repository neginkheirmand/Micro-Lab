#include <Wire.h>

#define ADDR_Ax 0b000 //A2, A1, A0
#define ADDR (0b1010 << 3) + ADDR_Ax //creating the address of the slave eeprom (the signals that define which slave and in what type of communication R/W)

int flag = 0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  writeI2CByte(0, 1);
  Serial.println(readI2CByte(0));
  
}

void loop() {
  if(flag==0){
    for (byte i = 0; i <= 10; i+=2) {
      writeI2CByte(i, i+1);
      Serial.println(i, i+1);
    }
    Serial.println();
    flag++;
  }else if(flag==1){
    for (byte i = 0; i <= 10; i+=2) {
      byte a = readI2CByte(i);
      Serial.println(i, a);
    }
    flag++;
  } 
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
