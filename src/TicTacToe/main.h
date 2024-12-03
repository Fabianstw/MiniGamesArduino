#ifndef MAIN_H
#define MAIN_H

#include <Keypad.h>

#include "../Menu/menu.h"
#include "../pitches.h"
#include "../utils/utils.h"
#include "Arduino.h"
#include "TicTacToe.h"
#include "U8glib.h"

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
 public:
  TicTacToeGame(int buzzerPin, int redLightPin, int yellowLightPin,
                int greenLightPin, U8GLIB_SH1106_128X64 u8g, Keypad &keypad,
                GameMenu gameMenu);

  void mainLoopTic();

 private:
  int buzzerPin;
  int redLightPin;
  int yellowLightPin;
  int greenLightPin;
  U8GLIB_SH1106_128X64 u8g;
  Keypad &keypad;
  GameMenu gameMenu;

  void playMusic(int melody[], int noteDurations[], int size);
  void setupTicTacToe();
  void initializeBoard();
  void drawMainDisplay(void);
  void drawGameStatus();
  void drawGameStandings();
  void drawPlayersTurn();
  void drawBoard();
  void drawMenu(void);
  void resetLights();
  void getUserInputGame();
  void updateBoard(int row, int col);
  void playPlayerMoveSound();
  void getUserInputMenu();
  void getAIInput();
  void getUserInputStanding();
  void resetGame();

  void checkForGameEnding();
  void checkForDraw();
  void checkForWinner();

  void loopEnd();
  void loopGame();
  void loopMenu();

  void playGameEndMusic();
};

#endif  // MAIN_H