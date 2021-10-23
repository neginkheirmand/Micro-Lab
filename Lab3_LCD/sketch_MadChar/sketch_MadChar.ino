
/*
 This sketch prints a moving char to the LCD

  The circuit:
  ####LCD####
 * LCD pin 1(vss) to gnd
 * LCD pin 2(vcc) to 220Ohm resistor (resistors other pin is connected to 5V)
 * LCD pin 3(V0 -display contrast pin-) to gnd
 * LCD pin 4(RS -register select-) to D6 (arduino)
 * LCD pin 5(R/w) to gnd
 * LCD pin 6(Enable)  to D7 (arduino)
 * LCD pin 7 (Data0) -> unused 
 * LCD pin 8 (Data1) -> unused
 * LCD pin 9 (Data2) -> unused
 * LCD pin 10(Data3) -> unused
 * LCD pin 11(Data4) to D2 (arduino)
 * LCD pin 12(Data5) to D3 (arduino)
 * LCD pin 13(Data6) to D4 (arduino)
 * LCD pin 14(Data7) to D5 (arduino)
 * LCD pin 15(Anode) to 10K potentiometer's middle pin(output)->I actually used a 100k potentiometer and changed its value in range of 1k to 10k only (i didnt have a 10k pot)
 * LCD pin 16(Cathode) to 5v
 
*/

// include the library code:
#include <LiquidCrystal.h>

const int rs = 6, en = 7, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char mad_char = '*'; // the default value is a star
int rowPosition = 0;
int colPosition = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.clear();
  delay(1000);
  lcd.setCursor(6,1);
  lcd.print("MADCHAR");
  delay(1500);
  lcd.clear();
}

void loop() {
  rowPosition++;
  colPosition++;
  if(colPosition==20){
    colPosition=0;
  }
  if(rowPosition==4){
    rowPosition=0;
  }
  lcd.clear();
  delay(400);
  lcd.setCursor(colPosition, rowPosition);
  lcd.print(mad_char);
  delay(400);
}
 
