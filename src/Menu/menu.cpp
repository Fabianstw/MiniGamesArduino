#include "menu.h"

#include <Keypad.h>

#include "Arduino.h"
#include "U8glib.h"

GameMenu::currentGameChoice GameMenu::gameCH = MENUGAMECHOICE;

/**
 * This is the constructor for the GameMenu class
 * @param u8g The display object
 * @param keypad The keypad object
 */
GameMenu::GameMenu(U8GLIB_SSD1306_128X64 &u8g, Keypad &keypad)
    : u8g(u8g), keypad(keypad) {}

/**
 * This function draws the menu options on the screen
 */
void GameMenu::drawMenuOptions() {
  u8g.setFont(u8g_font_tpssb);
  u8g.drawStr(0, 10, "Welcome to MiniGames");
  u8g.drawStr(0, 20, "1: Tic Tac Toe");
  u8g.drawStr(0, 30, "2: Simon Says");
  u8g.drawStr(0, 40, "3: Music");
}

/**
 * This function gets the user input from the keypad
 * and sets the game choice accordingly
 */
void GameMenu::getUserInputMenu() {
  char key = keypad.getKey();
  if (key == '1') {
    gameCH = TICTACTOE;
  } else if (key == '2') {
    gameCH = SIMONSAYS;
  } else if (key == '3') {
    gameCH = MUSIC;
  }
}

/**
 * This function returns the current game choice
 */
GameMenu::currentGameChoice GameMenu::getGameChoice() { return gameCH; }

/**
 * This function sets to a "new" game choice
 * @param choice The new game choice
 */
void GameMenu::setGameChoice(currentGameChoice choice) { gameCH = choice; }

/**
 * This displays the menu (as long as in the menu) and
 * waits for the user to select a game
 */
void GameMenu::mainLoopMenu() {
  while (gameCH == MENUGAMECHOICE) {
    u8g.firstPage();
    do {
      drawMenuOptions();
    } while (u8g.nextPage());
    getUserInputMenu();
  }
}