#include <Servo.h> 
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {6, 5, 4, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8, 9, 10}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Declare the Servo pin
int servoPin = 3;
// Create a servo object
Servo servo;

String input = "0";

void setup() {
  Serial.begin(9600);
  // We need to attach the servo to the used pin number
  servo.attach(servoPin);
//  servo.writeMicroseconds(1500);
  delay(1000);
}

void loop() {
  char keyPressed = keypad.waitForKey();
  String s1 = "0123456789";
    if (s1.indexOf(keyPressed)!=-1){
      //is a number
      if(input=="0"){
        if(keyPressed!='0'){
          input = String(keyPressed);
        }
      }else{
        input = String(input + String(keyPressed));
      }
    }else if( keyPressed =='C'){
      //is the delete key
      input = input.substring(0, input.length()-1);
    }else if( keyPressed =='D'){
      //is the empty key
      input="0";
    }else if( keyPressed =='B'){
      //is the "show me" key
      long a = atol(input.c_str()); 
      if(a<=180 && a >=0){
        servo.write(0);
        delay(2000);
        servo.writeMicroseconds(1000);
        servo.write(a);
        Serial.print("servo is showing the angle: " );  
        Serial.println(a);  
      }else if(a>180&& a<=360){
        servo.writeMicroseconds(1000);
        servo.write(180);
        delay(2000);
        servo.writeMicroseconds(2000);
        a = 360-a;
        servo.write(a);
        Serial.print("servo is showing the angle: " );  
        Serial.println(input);  
        Serial.print("a is  " );  
        Serial.println(a);  
      }else{
        Serial.print("servo cant show the angle: ");  
        Serial.println(a);  
      }
    }
  Serial.println(input);  
}
