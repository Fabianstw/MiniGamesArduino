#include "mainMusic.h"

#include <Keypad.h>

#include "../pitches.h"
#include "../utils/utils.h"
#include "Arduino.h"
#include "U8glib.h"
#include "songs.h"

MainMusic::MainMusic(U8GLIB_SH1106_128X64 &u8g, Keypad &keypad, int buzzerPin)
    : u8g(u8g), keypad(keypad), buzzerPin(buzzerPin) {
  options[0] = "Star Wars";
  options[1] = "Mario";
  options[2] = "Tetris Long";
  options[3] = "Alle meine Entchen";
}

/**
 * Function to draw the music options
 * Scrollbar through the options
 */
void MainMusic::drawMusicOptions() {
  u8g.setFont(u8g_font_6x10);
  for (int i = 0; i < displayLines; i++) {
    int index = scrollOffset + i;
    if (index < numOptions) {
      u8g.drawStr(0, (i + 1) * 10, (String(i) + ": " + options[index]).c_str());
    }
  }
  u8g.drawStr(0, 58, "8 Down, 9 Up");
}

/**
 * Function to scroll up the menu
 */
void MainMusic::scrollUp() {
  if (scrollOffset > 0) {
    scrollOffset--;
  }
}

/**
 * Function to scroll down the menu
 */
void MainMusic::scrollDown() {
  if (scrollOffset < numOptions - displayLines) {
    scrollOffset++;
  }
}

/**
 * Function to play the melody
 */
void MainMusic::playMeldoy(int melodyAndDuration[], int melodySize) {
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
 * Function to draw the song title
 */
void MainMusic::drawSongTitel() {
  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(0, 10, "Playing:");
  u8g.drawStr(0, 20, options[choosenSong]);
}

/**
 * Function to get the input from the user
 */
void MainMusic::getScrollInput() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    switch (key) {
      case '0':
      case '1':
      case '2':
      case '3':
        choosenSong = scrollOffset + (key - '0');
        musicPhase = MUSICPLAY;
        break;
      case '8':
        scrollDown();
        break;
      case '9':
        scrollUp();
        break;
      default:
        break;
    }
  }
}

int delayPlaySong = 0;

/**
 * Function to play the main loop of the music
 * Using delay to play the melody
 *  So the title can be drawn correctly
 */
void MainMusic::mainLoopMusic() {
  u8g.firstPage();
  do {
    if (musicPhase == MUSICCHOICE) {
      drawMusicOptions();
      getScrollInput();
    } else {
      // play the melody
      drawSongTitel();
      if (delayPlaySong == 50) {
        playMeldoy(melodies[choosenSong - 1], melodyLengths[choosenSong - 1]);
      } else if (delayPlaySong > 100) {
        delayPlaySong = 0;
        musicPhase = MUSICCHOICE;
      }
      delayPlaySong++;
    }
  } while (u8g.nextPage());
}