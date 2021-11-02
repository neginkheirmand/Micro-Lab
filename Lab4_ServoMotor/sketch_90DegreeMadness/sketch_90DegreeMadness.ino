#include <Servo.h> 


// Declare the Servo pin
int servoPin = 3;
// Create a servo object
Servo servo;

void setup() {
  Serial.begin(9600);
  // We need to attach the servo to the used pin number
  servo.attach(servoPin);
}

void loop() {
  // Make servo go to 0 degrees
  servo.write(0);
  Serial.println("0");
  delay(1000);
  // Make servo go to 90 degrees
  servo.write(90);
  Serial.println("90");
  delay(1000);

}
