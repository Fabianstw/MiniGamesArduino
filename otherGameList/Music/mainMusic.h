#ifndef MAINMUSIC_H
#define MAINMUSIC_H

#include <Keypad.h>

#include "Arduino.h"
#include "U8glib.h"

/**
 * Structure of the class for them usic
 */
class MainMusic {
 public:
  MainMusic(U8GLIB_SH1106_128X64 &u8g, Keypad &keypad, int buzzerPin);
  void mainLoopMusic();

 private:
  void drawMusicOptions();
  void getUserInputMusic();

  void scrollUp();
  void scrollDown();
  void getScrollInput();

  void drawSongTitel();
  void playMeldoy(int melodyAndDuration[], int melodySize);

  enum MUSICPHASE { MUSICCHOICE, MUSICPLAY };
  MUSICPHASE musicPhase = MUSICCHOICE;

  U8GLIB_SH1106_128X64 &u8g;
  Keypad &keypad;
  int buzzerPin;
  // Define the number of items and the display settings
  const int numOptions = 5;
  const char *options[10];
  const int displayLines = 4;
  int scrollOffset = 0;
  int choosenSong = 0;
};

#endif