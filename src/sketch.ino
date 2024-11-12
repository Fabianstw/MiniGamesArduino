#include <Keypad.h>

#include "Arduino.h"
#include "Menu/menu.h"
#include "SimonSays/SSmain.h"
#include "TicTacToe/main.h"
#include "U8glib.h"
#include "utils/utils.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK |
                          U8G_I2C_OPT_FAST);

const uint8_t KEYPAD_ROWS = 4;
const uint8_t KEYPAD_COLS = 4;
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {{'1', '2', '3', 'A'},
                                       {'4', '5', '6', 'B'},
                                       {'7', '8', '9', 'C'},
                                       {'*', '0', '#', 'D'}};
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};

Keypad keypad =
    Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

int buzzerPin = 13;
int redLightPin = 7;
int blueLightPin = 8;
int yellowLightPin = 9;
int greenLightPin = 12;

GameMenu gameMenu = GameMenu(u8g, keypad);

/**
 * Setups the pinModes for each light
 */
void setupPinModes() {
  pinMode(redLightPin, OUTPUT);
  pinMode(yellowLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(blueLightPin, OUTPUT);
}

void setup() {
  initRandomSeed();
  setupPinModes();

  u8g.setFont(u8g_font_tpssb);
  u8g.setColorIndex(1);

  Serial.begin(9600);
}

void loop() {
  if (gameMenu.getGameChoice() == GameMenu::MENUGAMECHOICE) {
    gameMenu.mainLoopMenu();
  } else if (gameMenu.getGameChoice() == GameMenu::TICTACTOE) {
    TicTacToeGame ticTacToeGame =
        TicTacToeGame(buzzerPin, redLightPin, yellowLightPin, greenLightPin,
                      u8g, keypad, gameMenu);
    ticTacToeGame.mainLoopTic();
  } else if (gameMenu.getGameChoice() == GameMenu::SIMONSAYS) {
    SimonSaysGame simonSaysGame =
        SimonSaysGame(u8g, keypad, buzzerPin, redLightPin, yellowLightPin,
                      greenLightPin, blueLightPin, gameMenu);
    simonSaysGame.mainLoopSimon();
  }
}
