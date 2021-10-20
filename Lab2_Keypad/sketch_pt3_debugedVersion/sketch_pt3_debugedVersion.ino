#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
String number = "";

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();

  if (key != NO_KEY){
    String s1 = "0123456789";
    if (s1.indexOf(key)!=-1){
      if(number.equals("0")){
        number = "";
      }
      number = String(number + String(key));
      if(number.length()>1){
        Serial.println(String("invalid number!  number: ")+String(number));        
      }else{
        Serial.println(String("number: "+String(number)));        
      }
    }
    else if(key='C'){//the C is the delete key
      number = "";
      Serial.println("pressed the delete key, now the number is 0");      
    }
    else{
      Serial.println("Please enter a number!");
      number = "";//set to default
    }
  }
}
