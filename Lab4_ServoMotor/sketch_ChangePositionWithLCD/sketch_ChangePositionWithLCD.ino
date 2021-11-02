#include <Servo.h> 

// Declare the Servo pin
int servoPin = 3;
// Create a servo object
Servo servo;

String input = "0";
int incomingByte = 0; // for incoming serial data

void setup() {
  Serial.begin(9600);
  // We need to attach the servo to the used pin number
  servo.attach(servoPin);
  delay(1000);
}

void loop() {
  if (Serial.available() > 0) {
    input = Serial.readString();
    Serial.println(input);
    int a = input.toInt();
    //if cant be casted to int the return value will be 0
    if( a!=0 ){
      int degreeNow = servo.read();
      if(degreeNow+a<=180 and degreeNow+a>=0){
        if(a>0){
          servo.writeMicroseconds(1000);
          servo.write(degreeNow+a);
        }else if(a<0){
          servo.writeMicroseconds(2000);
          servo.write(degreeNow+a);
        }
          Serial.println("degree now is ");
          Serial.println(degreeNow+a);
      }
    }
  }
}
