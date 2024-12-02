int analogPin = A3;
                    
int val = 0;  // variable to store the value read

void setup() {
  Serial.begin(9600);
}

void loop() {
  val = analogRead(analogPin);  // read the input pin
  Serial.println(val);          // debug value
}
