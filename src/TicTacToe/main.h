#ifndef MAIN_H
#define MAIN_H

#include <ArduinoSTL.h>
#include <Keypad.h>

#include <array>

#include "../Menu/menu.h"
#include "../utils/utils.h"
#include "Arduino.h"
#include "TicTacToe.h"
#include "U8glib.h"
#include "pitches.h"

using namespace std;

// notes in the melody:
extern int startingGame[];
extern int startingGameNoteDurations[];
extern int startingGameSize;

extern int player0MoveNote;
extern int player0MoveNoteDuration;

extern int player1MoveNote;
extern int player1MoveNoteDuration;

extern int winMelody[];
extern int winMelodyNoteDurations[];
extern int winMelodySize;

extern int drawMelody[];
extern int drawMelodyNoteDurations[];
extern int drawMelodySize;

extern bool playWinDrawMelody;

typedef enum {
  MENU,
  STANDING_X,
  STANDING_O,
  DRAW,
  GAMEPVSP,
  GAMEPVSAI,
} GameStatus;

extern int player;
extern int counterLastMove;
extern int player1Wins;
extern int player2Wins;
extern int moveCoutner;
extern GameStatus gameStatus;
extern char board[3][3][2];

extern bool boardDrawn;

class TicTacToeGame {
 private:
  int buzzerPin;
  int redLightPin;
  int yellowLightPin;
  int greenLightPin;
  U8GLIB_SSD1306_128X64 u8g;
  Keypad &keypad;
  GameMenu gameMenu;

 public:
  TicTacToeGame(int buzzerPin, int redLightPin, int yellowLightPin,
                int greenLightPin, U8GLIB_SSD1306_128X64 u8g, Keypad &keypad,
                GameMenu gameMenu);
  void playMusic(int melody[], int noteDurations[], int size);
  void setupTicTacToe();
  void initializeBoard();
  void drawBoard(void);
  void drawMenu(void);
  void resetLights();
  void getUserInputGame();
  void getUserInputMenu();
  void getAIInput();
  void getUserInputStanding();
  void mainLoopTic();
};

#endif  // MAIN_H