const byte ledPin1 = 13;
const byte ledPin2 = 12;

const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
const char myName[] = "negin";
const int nameSize = strlen(myName);

volatile byte state = LOW;
volatile int counter = strlen(myName) + 1;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  
//  attachInterrupt(digitalPinToInterrupt(interruptPin1), spellMyName, FALLING);// when an interrupt is received for pin d2 the lights will turn on
  attachInterrupt(digitalPinToInterrupt(interruptPin1), turnOn, FALLING);// when an interrupt is received for pin d2 the lights will turn on
  attachInterrupt(digitalPinToInterrupt(interruptPin2), turnOff, FALLING);// when an interrupt is received for pin d2 the lights will turn off

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println(nameSize);
}

void loop() {
  while (counter < nameSize) {
    delay(1000);  
    state = !state;
    digitalWrite(ledPin1, state);
    digitalWrite(ledPin2, state);
    counter++;
    Serial.println(counter);
  }
}

void turnOn() {
//  state = !state;
  state = LOW;
  digitalWrite(ledPin1, state);
  digitalWrite(ledPin2, state);
}


void turnOff() {
//  state = !state;
  state = HIGH;
  digitalWrite(ledPin1, state);
  digitalWrite(ledPin2, state);
}

void spellMyName(){
  counter = 0;
  turnOff();
}
