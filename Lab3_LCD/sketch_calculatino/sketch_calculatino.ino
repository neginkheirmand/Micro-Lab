/*
 This sketch is a simple calculator(supports only one operator or less)

  The LCD circuit:
 
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
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','+','-'}, //A is used as + and B is used as -
  {'/','*','E','F'}  //C is used as /     D is used as *     E is used to delete the last char and F is used as =
};
byte rowPins[ROWS] = {12, 16, 17, 18}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {11, 10, 9, 8}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int rs = 6, en = 7, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String input = "";


void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.clear();
  delay(1000);
  lcd.print("Calculatino");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(1000);
  lcd.clear();
}

String addition(long a, long b){
  return String(a + b);
}

String subtraction(long a, long b){
  return String(a - b);
}

String division(long a, long b){
  if( b !=0){
    return String(a/b);
  }
  input = "";
  return "cant divide by 0";
}

String multiplication(long a, long b){
   return String(a*b);
}

String arithmeticLogic(String x){
    if(x.indexOf('+')!=-1){
      //its addition
      int pos = x.indexOf('+');
      long a = atol(x.substring(0, pos).c_str());
      long b = atol(x.substring(pos+1, input.length()).c_str());
      return addition(a, b);
      
    }else if (x.indexOf('-')!=-1){
      //its subtraction
      int pos = x.indexOf('-');
      long a = atol(x.substring(0, pos).c_str());
      long b = atol(x.substring(pos+1, input.length()).c_str());
      return subtraction(a, b);
      
    }else if(x.indexOf('/')!=-1){
      //its division
      int pos = x.indexOf('/');
      long a = atol(x.substring(0, pos).c_str());
      long b = atol(x.substring(pos+1, input.length()).c_str());
      return division(a, b);
      
    }else if(x.indexOf('*')!=-1){
      //its multiplication
      int pos = x.indexOf('*');
      long a = atol(x.substring(0, pos).c_str());
      long b = atol(x.substring(pos+1, input.length()).c_str());
      return multiplication(a, b);
      
    }else{
      return x;
    }
}

void loop() {
  char key = keypad.getKey();
  lcd.setCursor(0, 0);
  if (key != NO_KEY){
    String s1 = "0123456789";
    if (s1.indexOf(key)!=-1|| key =='+' || key =='-' || key =='/' || key =='*'){
      input = String(input + String(key));
      if(input.length()!=0){
        lcd.clear();
      delay(50);
        lcd.print(input);
      }
    }else if( key =='E'){
      //delete key pressed
      if (input.length()!=0){
        input = input.substring(0, input.length()-1);
      }
      lcd.clear();
      delay(50);
      lcd.print(input);
      
    }else if( key =='F'){
      // = key pressed, start calculating
      input = arithmeticLogic(input);
      lcd.clear();
      delay(50);
      lcd.print(input);
    }
  }
}
 
