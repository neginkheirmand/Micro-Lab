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

const byte ledPin1 = 13;
const byte ledPin2 = 12;
const byte ledPin3 = 11;
const byte ledPin4 = 10;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  for (int i = 10; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop(){
  char key = keypad.getKey();

  if (key != NO_KEY){
    if (key=='0'|| key=='1' || key=='2' || key=='3' || key=='4'){
      String keyPressed = String(key);
      int keyNum = keyPressed.toInt();
      for (int i = 10; i <= 13; i++) {
        if( i>=10+keyNum ){
          digitalWrite(i, HIGH);// turn LED off
        }else{
          digitalWrite(i, LOW);// turn LED on
        }
      }
    }
    else if(key=='5' || key=='6' || key=='7' || key=='8' || key=='9'){
      String keyPressed = String(key);
      String output = String(keyPressed + " LEDs are turned on!");
      int keyNum = keyPressed.toInt();
      String LEDs = "";
      for (int i = 1; i <= 9; i++) {
        if(i<=keyNum){
          LEDs = String(LEDs + "★");
        }else{
          LEDs = String(LEDs + "☆");
        }
      }
      output = String(output + LEDs);
      Serial.println(output);
    }
  }
}
