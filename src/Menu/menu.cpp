#include "U8glib.h"
#include <Keypad.h>
#include "Arduino.h"

#include "menu.h"

GameMenu::gameChoice GameMenu::gameCH = MENUGAMECHOICE;

GameMenu::GameMenu(U8GLIB_SSD1306_128X64 &u8g, Keypad &keypad) : u8g(u8g), keypad(keypad) {}

void GameMenu::drawMenuOptions()
{
    u8g.setFont(u8g_font_tpssb);
    u8g.drawStr(0, 10, "Welcome to MiniGames");
    u8g.drawStr(0, 20, "1: Tic Tac Toe");
    u8g.drawStr(0, 30, "2: Snake");
}

void GameMenu::getUserInputMenu()
{
    char key = keypad.getKey();
    if (key == '1')
    {
        gameCH = TICTACTOE;
    }
    else if (key == '2')
    {
        gameCH = SNAKE;
    }
}

GameMenu::gameChoice GameMenu::getGameChoice()
{
    return gameCH;
}

void GameMenu::mainLoopMenu()
{
    while (gameCH == MENUGAMECHOICE)
    {
        u8g.firstPage();
        do
        {
            drawMenuOptions();
        } while (u8g.nextPage());
        getUserInputMenu(); // Check for user input to update gameCH
    }
}