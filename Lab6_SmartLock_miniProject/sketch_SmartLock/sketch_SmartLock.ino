
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'}, 
  {'C','D','E','F'}  //E is used to delete the last char and F is used as get result
};
byte rowPins[ROWS] = {7, A0, A1, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 4, 8}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String input = ""; 


bool getPass = false; 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Smart Lock Ready");
  Serial.println();

}
void loop() 
{
  if(getPass){
    char key = keypad.getKey();
      if (key != NO_KEY){
        if( key =='E'){
          //delete key pressed
          if (input.length()!=0){
            input = input.substring(0, input.length()-1);
          }
          Serial.println(input);
        }else if( key =='F'){
          // = key pressed, start calculating
          if(input =="123CD"){
            Serial.println("correct pass!");
          }else{
            Serial.println("incorrect pass!");
          }
          input="";
          getPass = false;
        }else{
          input = String(input + String(key));
          Serial.println(input);
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
      input = "";
      getPass = true;
      
    }else{
      Serial.println(" Access denied");
      delay(3000);
    }
  }
} 
