#include <Servo.h> 

// Declare the Servo pin
int servoPin = 3;

int analogPin = 3;
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
  long value = analogRead(analogPin);
  value = map(value, 0,1023,0,180);
  servo.write(value);
  Serial.println(value);
}
