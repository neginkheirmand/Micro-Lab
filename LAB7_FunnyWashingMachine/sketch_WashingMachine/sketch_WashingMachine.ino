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


// The washing machine will have two modes, the default mode witch is 1->2->3->4 and then a custome which can be defined by the user
#include <Wire.h>

#include <LiquidCrystal.h>
#include <Keypad.h>

#define ADDR_Ax 0b000 //A2, A1, A0
#define ADDR (0b1010 << 3) + ADDR_Ax //creating the address of the slave eeprom (the signals that define which slave and in what type of communication R/W)

const int rs = 6, en = 7, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const byte ROWS = 2; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','4'},
  {'M','P','S','C'},// M is new configuration, P is Pause (if is in progress and if its already stoped will play on), S is start/end button, C is delete key
};
byte rowPins[ROWS] = {8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 11, 12, A1}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


int state = 0;
//+--------------------+--------+
//|        stat        | number |
//+--------------------+--------+
//| idle(in the menu)  |      0 |
//| pause              |      1 |
//| working            |      2 |
//| gettingStarted     |      3 |
//| gettingCustom      |      4 |
//+--------------------+--------+



int mode = 6;
//  +-------------+--------------+----------------+
//  | mode number | description  | LED pin number |
//  +-------------+--------------+----------------+
//  |           1 | prewash      | D13            |
//  |           2 | soap-wash    | A2             |
//  |           3 | water-wash   | A3             |
//  |           4 | dry          | A0             |
//  |           5 | done         | all LEDs       |
//  |           6 | not working  | -              |
//  +-------------+--------------+----------------+



int phaseTime = 3500;
unsigned long workTime = 0 ;

String input = "";

int defaultSize;
int customSize;

int indexOfWork = -1;
bool flagDefault = true;

bool flagShowLcd = true;

byte readI2CByte(byte data_addr);
void writeI2CByte(byte data_addr, byte data);

void loadArr(int arr[],int sizeArr, int address){
  for(int i=0; i<sizeArr; i++){
    byte a = readI2CByte(address+i);
    arr[i]= (int)a;
  }
}

void saveArr(int arr[], int sizeArr){
  //the address for saving is always 6
  writeI2CByte(6, sizeArr);
  Serial.println("should save:");
  for(int i=0; i<sizeArr; i++){
    writeI2CByte(7+i, arr[i]);
    Serial.println(arr[i]);
  }
  Serial.println("saved:");
  for(int i=0; i<sizeArr; i++){
    Serial.println(readI2CByte(7+i));
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(20, 4);
  lcd.clear();
  delay(1000);
  lcd.print("Washing Machine");
  //and the LEDs
  pinMode(13, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A0, OUTPUT);
  
  delay(1000);
  
  //load the sizes of the arr
  defaultSize = readI2CByte(0);
  customSize = readI2CByte(6);

  //load the last saved state and mode 
  state = readI2CByte(15);
  mode = readI2CByte(16);
  //also load the index and the optional configuration -> if the machine was paused and then turned off at that state, should continue the process
  indexOfWork = readI2CByte(17);
  flagDefault = readI2CByte(18);
  changeState_mode(state, mode);
  
}

void setUpLED(){
  if(state==2){//machine is working
    if (mode==1){
      digitalWrite(13, HIGH);
      analogWrite(A2, 0);
      analogWrite(A3, 0);
      analogWrite(A0, 0);
    }else if(mode==2){
      digitalWrite(13, LOW);
      analogWrite(A2, 255);
      analogWrite(A3, 0);
      analogWrite(A0, 0);
    }else if(mode==3){
      digitalWrite(13, LOW);
      analogWrite(A2, 0);
      analogWrite(A3, 255);
      analogWrite(A0, 0);
    }else if(mode==4){
      digitalWrite(13, LOW);
      analogWrite(A2, 0);
      analogWrite(A3, 0);
      analogWrite(A0, 255);
    }else if(mode==5){ //done
      digitalWrite(13, HIGH);
      analogWrite(A2, 255);
      analogWrite(A3, 255);
      analogWrite(A0, 255);
    }else if(mode==6){ //not working
      digitalWrite(13, LOW);
      analogWrite(A2, 0);
      analogWrite(A3, 0);
      analogWrite(A0, 0);
    }
  }else{//not working
      digitalWrite(13, LOW);
      analogWrite(A2, 0);
      analogWrite(A3, 0);
      analogWrite(A0, 0);
  }
}

void printLCD(String str){
  lcd.begin(20, 4);
  lcd.clear();
  delay(1000);
  lcd.print(str);
  Serial.println(str);
}

void changeState_mode(int newState, int newMode){
  flagShowLcd=true;
  state = newState;
  
  writeI2CByte(17, indexOfWork);
  writeI2CByte(18, flagDefault);
  
  String displayText = "error";
  if (newMode<7 &&newMode>0 && state == 2){
    mode = newMode; 
  }
  
  // save the new state and mode in the EEPROM
  // the state will be saved at 15 and the mode will be saved at 16
  //since there is sizeDefaultArr(1)+defaultArr(4)+sizeCustomArr(1)+customArr(maximum = 8) which is 14 bytes of data so the byte 15 and 16 are empty
  writeI2CByte(15, state);
  writeI2CByte(16, mode);
  
  if(state==0){
    displayText = "idle(in the menu)";
  }else if(state ==1){
    displayText = "Paused";
  }else if(state==2){
    if(mode >0 && mode<5){
      displayText = "working in mode " + String( mode);
    }else if(mode ==5){
      displayText = "DONE!";
    }else if(mode ==6){
      displayText = "Not Working!";
    }
  }else if(state==3){
    displayText = "Getting Started state";
  }else if(state==4){
    displayText = "Getting Started state";
  }
  printLCD(displayText);
  setUpLED();//Show new state and mode in the led
  delay(100);
}

void loop() {

  if(keypad.getKey()=='P' && state==2 &&  mode!=6){ //pressed pause button when the machine was actually working
    changeState_mode(1, -1);//this will change the state to pause but not change the mode so that later when the state goes back to working the mode is still there
    printLCD("System Paused at state = "+String(state)+"and mode = "+String(mode));
  }else{
    if(state==0){//idle
      if(flagShowLcd){
        printLCD("S: start and                            M: create a new custom config");
      }
      char key = keypad.getKey();
      if(key == 'S'){//pressed start
        printLCD("Which type of washing? 1)default? or 2)custom? ");
        delay(1000);
        changeState_mode(3, -1) ; 

      }else if(key == 'M'){
          changeState_mode(4, -1) ;
          printLCD("Please enter the custom washing technique's protocol order");
      }
      if(state==0){
        flagShowLcd=false;
      }
    }else if(state==1){//pause
      char key = keypad.waitForKey();
      //continue if the unpause is pressed
      if(key =='P'){
        changeState_mode(2, -1);//change the state to working and mode back to what it was
      }
    }else if(state == 2){//working
      if(millis() - workTime >=phaseTime){

        
        int sizeWork = defaultSize;
        if(!flagDefault){
          sizeWork = customSize;
        }
        
        if(indexOfWork<sizeWork-1){//there is still work left to do
          indexOfWork++;
          workTime=millis();
          if(flagDefault){
            int defaultArr[defaultSize];
            loadArr(defaultArr, defaultSize, 1);
            changeState_mode(2, defaultArr[indexOfWork]);
          }else{
            int customArr[customSize];
            loadArr(customArr, customSize, 7);
            changeState_mode(2, customArr[indexOfWork]);
          }
        }else{//done working
          if(flagShowLcd){
            changeState_mode(2, 5);
            delay(1000);
            printLCD("Enter S to end the process");
            flagShowLcd=false;
          }
          
          char key = keypad.getKey();
          if(key=='S'){//to close the process
            changeState_mode(0, 6);
            indexOfWork=0;
          }
        }
        
      }
    }else if(state == 3){//gettingStarted
      if(flagShowLcd){
        //show the options which are the default and then the custom 
        int defaultArr[defaultSize];
        loadArr(defaultArr, defaultSize, 1);
        String temp = "1) ";
        for(int i=0; i<defaultSize; i++){
          temp=temp+String(defaultArr[i]);
          if(i!=defaultSize-1){
            temp= temp +"->";
          }
        }
        int customArr[customSize];
        loadArr(customArr, customSize, 7);
        temp = temp + "       2) ";
        for(int i=0; i<customSize; i++){
          temp=temp+String(customArr[i]);
          if(i!=customSize-1){
            temp= temp +"->";
          }
        }
        lcd.clear();
        lcd.print(temp);
        Serial.println(temp);
        delay(1000);
        flagShowLcd=false;
      }
      char key = keypad.getKey();
      if(key=='1'){
        //the default
        //TODO 
        flagDefault = true;
        indexOfWork = 0;
        workTime = millis();
        int defaultArr[defaultSize];
        loadArr(defaultArr, defaultSize, 1);
        changeState_mode(2, defaultArr[indexOfWork]);
      }else if(key=='2'){
        //the custom
        //TODO
        flagDefault = false;
        indexOfWork = 0;
        workTime = millis();
        int customArr[customSize];
        loadArr(customArr, customSize, 1);
        changeState_mode(2, customArr[indexOfWork]);
      }else if(key=='C'){
        //the back button
        changeState_mode(0, 6) ; //put it back to idle mode
      }else if(key!=NO_KEY){
        printLCD("Please choose between the options 1 or 2, if you want to cancel the process press C");
      }
    }else if(state == 4){//gettingCustom
    //get input and save into the eeprom
    char key = keypad.waitForKey();
    String s1 = "1234";
    if(s1.indexOf(key)!=-1){//entered number
      input = String(input + String(key));
      if(input.length()!=0){
        printLCD(input);
      }
    }else if(key=='S'){
      printLCD(String("input was ")+input);
      customSize = input.length();
      int arrInput[customSize];
      Serial.println("but before that");
      for(int i=0; i<customSize; i++){
        arrInput[i]=input.charAt(i) - '0';
        Serial.println(arrInput[i]);
      }
      saveArr(arrInput, customSize);
      printLCD("saved the custom setting");
      delay(1000);
      changeState_mode(0, -1);
    }else if(key=='C'){
      if (input.length()!=0){
        input = input.substring(0, input.length()-1);
      }
      printLCD(input);
    }
  }
  }
}

void writeI2CByte(byte data_addr, byte data){
  Wire.beginTransmission(ADDR);
  delay(10);
  Wire.write(data_addr);
  delay(10);
  Wire.write(data);
  delay(10);
  Wire.endTransmission();
}

byte readI2CByte(byte data_addr){
  byte data = NULL;
  Wire.beginTransmission(ADDR);
  delay(10);
  Wire.write(data_addr);
  delay(10);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(ADDR, 1); //retrieve 1 returned byte
  delay(10);
  if(Wire.available()){
    data = Wire.read();
    delay(10);
  }
  return data;
}
