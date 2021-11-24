
/*
 This sketch is a simple calculator(supports only one operator or less)

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
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

const byte ROWS = 3; //three rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'}
};
byte rowPins[ROWS] = {8, A0, A1}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A2, A3, A4, A5}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const int rs = 6, en = 7, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String input = ""; 


bool getPass = false; 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Smart Lock Ready");
  Serial.println();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Lock Ready");

}
void loop() 
{
  if(getPass){
    char key = keypad.getKey();
      if (key != NO_KEY){
        if( key =='A'){
          //delete key pressed
          if (input.length()!=0){
            input = input.substring(0, input.length()-1);
          }
          Serial.println(input);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(input);
        }else if( key =='B'){
          // = key pressed, start calculating
          if(input =="9831023"){
            Serial.println("correct pass!");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("correct pass!");
            
          }else{
            Serial.println("incorrect pass!");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("incorrect pass!");
          }
          input="";
          getPass = false;
        }else{
          input = String(input + String(key));
          Serial.println(input);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(input);
        }
      }
  }else{
    
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "71 AD 89 1B") 
    {
      Serial.println("Card has Authorized access");
      Serial.println("Now please enter your pass");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Card has Authorized access");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Now please enter your pass");
      
      input = "";
      getPass = true;
      
    }else{
      Serial.println(" Access denied");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Access denied");
      
      delay(3000);
    }
  }
} 
