#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Keypad.h>

#include "Arduino.h"
#include "U8glib.h"

/**
 * Structure of the class for the game menu
 * Important so other classes / functions can access those functions and
 * variables
 */
class GameMenu {
 public:
  GameMenu(U8GLIB_SH1106_128X64 &u8g, Keypad &keypad);

  enum currentGameChoice { MENUGAMECHOICE, TICTACTOE, SIMONSAYS, MUSIC };

  void drawMenuOptions();
  void getUserInputMenu();
  void mainLoopMenu();
  void setGameChoice(currentGameChoice choice);

  U8GLIB_SH1106_128X64 &u8g;
  Keypad &keypad;

  static currentGameChoice gameCH;
  currentGameChoice getGameChoice();
};
#endif