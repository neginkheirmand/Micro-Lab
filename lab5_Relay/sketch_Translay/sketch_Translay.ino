const int base_transistor = 5;
int state = LOW;
void setup() {
  pinMode(base_transistor , OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(state == HIGH){
    state = LOW;
  }else{
    state = HIGH;
  }
  delay(2500);    //2.5 seconds
  digitalWrite(base_transistor, state);
  Serial.println(state);
}
