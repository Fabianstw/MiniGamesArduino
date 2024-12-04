/*
Music.ino (MiniGames Project)

This file can be used if the user wants to play different songs.

Inside the folder `arduino-songs-master` are a couple of different songs
which can be used inside songs.h.

`void mainLoopMusic() { playMeldoy(melodies[2], melodyLengths[2]); }`
select the corresponding index from the songs.g file to play your song.

@author: Fabian Stiewe
@data: 10.11.2024

Used libraries:
- U8g2lib
- Keypad
Those are listed in the libaries folder, but should be installed via the Arduino
IDE. (Not used here, but listed since they are included in the project.)

*/
#include <Keypad.h>
#include <U8g2lib.h>

#include "Arduino.h"
#include "pitches.h"
#include "songs.h"

int buzzerPin = 13;

void setup(void) {}

/**
 * #####################################################################
 *
 * Paste all game code below here
 *
 * #####################################################################
 */

/**
 * Function to play the melody
 */
void playMeldoy(int melodyAndDuration[], int melodySize) {
  // every second element is the duration / the note
  for (int i = 0; i < melodySize; i += 2) {
    int noteDuration = 900 / melodyAndDuration[i + 1];
    tone(buzzerPin, melodyAndDuration[i], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
  noTone(buzzerPin);
}

/**
 * Function to play the main loop of the music
 * Using delay to play the melody
 *  So the title can be drawn correctly
 */
void mainLoopMusic() { playMeldoy(melodies[2], melodyLengths[2]); }

/**
 * #####################################################################
 *
 * Call the game loop here
 *
 * #####################################################################
 */

void loop(void) {
  mainLoopMusic();
  delay(3000);
}