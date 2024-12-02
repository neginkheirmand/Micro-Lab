#include "pitches.h"
#include "ImperialMarch.h"



void setup() {
}

void loop() {
  for(int noteIndex=0; noteIndex<sizeof(noteDurations)/sizeof(int); noteIndex++){
    int duration = 1000/noteDurations[noteIndex];
    tone(8, melody[noteIndex], duration);
    delay((int)(duration*1.3)); //why is this line needed? because the board wont wait for the note to end and will move to the next line which changes the tone, so we need a delay to actually complete the previous tone before moving into the next one
  }
  noTone(8);
  delay(100);
}
