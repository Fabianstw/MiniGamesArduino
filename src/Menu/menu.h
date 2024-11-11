#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Keypad.h>
#include <U8glib.h>

#include "Arduino.h"

class GameMenu {
 public:
  GameMenu(U8GLIB_SSD1306_128X64 &u8g, Keypad &keypad);

  enum gameChoice { MENUGAMECHOICE, TICTACTOE, SIMONSAYS };

  void drawMenuOptions();
  void getUserInputMenu();
  void mainLoopMenu();
  void setGameChoice(gameChoice choice);

  U8GLIB_SSD1306_128X64 &u8g;
  Keypad &keypad;

  static gameChoice gameCH;
  gameChoice getGameChoice();
};
#endif