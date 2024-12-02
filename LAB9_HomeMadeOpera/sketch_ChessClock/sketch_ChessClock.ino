//this is a simple sketch for a chess clock with features like timing for these types of chess:
//Bulet->  1 min
//Blitz->  3 min
//Rapid-> 10 min

/*
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



int whiteMinutes  = 5;
int whiteSeconds  = 0;
int blackMinutes = 5;
int blackSeconds = 0;

bool turn = false; //when false is not the whites turn and when true its whites turn
bool playing = false;//when true the clock is running and the players are playing
// other variables for timekeeping
unsigned long lastTimeUpdate = 0; // micros() value when the clock was last updated

const int rs = 6, en = 7, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonState=-1;

void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  pinMode(10, INPUT);
  delay(500);
}

void printTime(){
    lcd.print("white time:"+String(whiteMinutes)+":"+String(whiteSeconds));
    lcd.print("black time:"+String(blackMinutes)+":"+String(blackSeconds));
}

void loop() {
  
    if(playing){
        buttonState = digitalRead(10);

        // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
        if (buttonState == HIGH) {
            // change turn
            turn=!turn;
        }

    // This next part is what makes our clock keep time.
    // First, we check if it is time to update the clock.
    // (We only update the clock once per second.)
        if ((micros() - lastTimeUpdate) >= 1000000UL) {
            
            if(turn){
                whiteSeconds-=1;
                if(whiteSeconds<0){
                    whiteMinutes-=1;
                    if(whiteMinutes<0){
                        tone(8, 2637, 1000);
                        lcd.print("white lost");
                        delay(1000);
                        playing=false;

                    }
                }
            }else{
                blackSeconds-=1;
                if(blackSeconds<0){
                    blackMinutes-=1;
                    if(blackMinutes<0){
                        tone(8, 2637, 1000);
                        lcd.print("black lost");
                        delay(1000);
                        playing=false;
                    }
                }
            }
            // And we schedule the next update for one second later.
            lastTimeUpdate += 1000000UL;  
            
        }
    }
  
}
