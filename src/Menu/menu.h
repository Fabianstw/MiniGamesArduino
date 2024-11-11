#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <U8glib.h>
#include <Keypad.h>
#include "Arduino.h"

class GameMenu
{
public:
    GameMenu(U8GLIB_SSD1306_128X64 &u8g, Keypad &keypad);
    void drawMenuOptions();
    void getUserInputMenu();
    void mainLoopMenu();

    U8GLIB_SSD1306_128X64 &u8g;
    Keypad &keypad;

    enum gameChoice
    {
        MENUGAMECHOICE,
        TICTACTOE,
        SNAKE
    };

    static gameChoice gameCH;
    gameChoice getGameChoice();
};
#endif