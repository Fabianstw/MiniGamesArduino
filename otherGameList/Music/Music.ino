/*
sketch.ino (MiniGames Project)

This file is the main file for the MiniGames project. It contains the setup and
loop functions for the Arduino. The setup function initializes the pins and
serial communication. The loop function contains the main game loop that
determines which game to play based on the user's input.

The MiniGames project contains two games: Tic Tac Toe and Simon Says, also some
music sound tracks.

@author: Fabian Stiewe
@data: 10.11.2024

Used libraries:
- U8glib
- Keypad
Those are listed in the libaries folder, but should be installed via the Arduino
IDE.

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