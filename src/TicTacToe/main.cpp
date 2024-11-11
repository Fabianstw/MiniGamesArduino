#include "main.h"

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
int startingGame[] = {NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
                      NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5};
int startingGameNoteDurations[] = {1, 3, 2, 4, 4, 4, 4, 4};
int startingGameSize = 8;

int player0MoveNote = NOTE_C5;
int player0MoveNoteDuration = 75;

int player1MoveNote = NOTE_A4;
int player1MoveNoteDuration = 150;

int winMelody[] = {NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4};
int winMelodyNoteDurations[] = {4, 4, 4, 4};
int winMelodySize = 4;

int drawMelody[] = {NOTE_C2, NOTE_G5, NOTE_A4, NOTE_A4};
int drawMelodyNoteDurations[] = {4, 8, 2, 4};
int drawMelodySize = 4;

bool playWinDrawMelody = true;

// random choose of the start player
int player = randInt(0, 2);
int counterLastMove = 0;
int player1Wins = 0;
int player2Wins = 0;
int moveCoutner = 0;
GameStatus gameStatus = MENU;
char board[3][3][2];

bool boardDrawn = false;

bool setupGame = false;

TicTacToeGame::TicTacToeGame(int buzzerPin, int redLightPin, int yellowLightPin,
                             int greenLightPin, U8GLIB_SSD1306_128X64 u8g,
                             Keypad &keypad, GameMenu gameMenu)
    : buzzerPin(buzzerPin),
      redLightPin(redLightPin),
      yellowLightPin(yellowLightPin),
      greenLightPin(greenLightPin),
      u8g(u8g),
      keypad(keypad),
      gameMenu(gameMenu) {
  if (!setupGame) {
    setupTicTacToe();
    setupGame = true;
  }
}

void TicTacToeGame::setupTicTacToe() {
  // Initialize the board
  initializeBoard();
  // Add any other setup code here
}

// Play music function with const parameters
void TicTacToeGame::playMusic(int *melody, int *noteDurations, int size) {
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(buzzerPin);
  }
}

// Function to initialize the board
void TicTacToeGame::initializeBoard() {
  strcpy(board[0][0], " ");
  strcpy(board[0][1], " ");
  strcpy(board[0][2], " ");

  strcpy(board[1][0], " ");
  strcpy(board[1][1], " ");
  strcpy(board[1][2], " ");

  strcpy(board[2][0], " ");
  strcpy(board[2][1], " ");
  strcpy(board[2][2], " ");
}

void TicTacToeGame::drawBoard(void) {
  u8g.setFont(u8g_font_tpssb);
  u8g.drawStr(0, 10, board[0][0]);
  u8g.drawStr(10, 10, "|");
  u8g.drawStr(20, 10, board[0][1]);
  u8g.drawStr(30, 10, "|");
  u8g.drawStr(40, 10, board[0][2]);

  u8g.drawStr(0, 20, "-");
  u8g.drawStr(10, 20, "|");
  u8g.drawStr(20, 20, "-");
  u8g.drawStr(30, 20, "|");
  u8g.drawStr(40, 20, "-");

  u8g.drawStr(0, 30, board[1][0]);
  u8g.drawStr(10, 30, "|");
  u8g.drawStr(20, 30, board[1][1]);
  u8g.drawStr(30, 30, "|");
  u8g.drawStr(40, 30, board[1][2]);

  u8g.drawStr(0, 40, "-");
  u8g.drawStr(10, 40, "|");
  u8g.drawStr(20, 40, "-");
  u8g.drawStr(30, 40, "|");
  u8g.drawStr(40, 40, "-");

  u8g.drawStr(0, 50, board[2][0]);
  u8g.drawStr(10, 50, "|");
  u8g.drawStr(20, 50, board[2][1]);
  u8g.drawStr(30, 50, "|");
  u8g.drawStr(40, 50, board[2][2]);

  // draw hows turn it is
  // P1: Y   P2:
  if (player == 0) {
    u8g.drawStr(9, 62, "P1: X");
  } else {
    u8g.drawStr(9, 62, "P2: O");
  }

  // draw game standing
  // P1: 0   P2: 0
  u8g.drawStr(60, 10, "Points:");
  u8g.drawStr(60, 22, "P1: ");
  u8g.drawStr(80, 22, String(player1Wins).c_str());
  u8g.drawStr(95, 22, "P2: ");
  u8g.drawStr(115, 22, String(player2Wins).c_str());

  if (gameStatus == STANDING_O) {
    u8g.drawStr(60, 62, "Player O wins!");
  } else if (gameStatus == STANDING_X) {
    u8g.drawStr(60, 40, "Player X wins!");
  } else if (gameStatus == DRAW) {
    u8g.drawStr(60, 62, "Draw!");
  }
}

void TicTacToeGame::drawMenu(void) {
  u8g.drawStr(0, 10, "Tic Tac Toe");
  u8g.drawStr(0, 20, "1: P vs P");
  u8g.drawStr(0, 30, "2: P vs AI");

  u8g.drawStr(0, 50, "3: Back to menu");
}

void TicTacToeGame::resetLights() {
  digitalWrite(redLightPin, LOW);
  digitalWrite(yellowLightPin, LOW);
  digitalWrite(greenLightPin, LOW);
}

void TicTacToeGame::getUserInputGame() {
  char key = keypad.getKey();
  // check if key between 1 and 9
  if (key >= '1' && key <= '9') {
    int position = key - '1';
    int row = position / 3;
    int col = position % 3;
    if (strcmp(board[row][col], " ") == 0) {
      if (player == 0) {
        strcpy(board[row][col], "X");
        player = 1;
      } else {
        strcpy(board[row][col], "O");
        player = 0;
      }
      moveCoutner++;
      if (player == 0) {
        tone(buzzerPin, player0MoveNote, player0MoveNoteDuration);
      } else {
        tone(buzzerPin, player1MoveNote, player1MoveNoteDuration);
      }
      return;
    }
  }
}

void TicTacToeGame::getUserInputMenu() {
  char key = keypad.getKey();
  if (key == '1') {
    gameStatus = GAMEPVSP;
    initializeBoard();
    return;
  }
  if (key == '2') {
    gameStatus = GAMEPVSAI;
    initializeBoard();
    return;
  }
  if (key == '3') {
    gameStatus = MENU;
    gameMenu.setGameChoice(gameMenu.MENUGAMECHOICE);
    return;
  }
}

void TicTacToeGame::getAIInput() {
  int position = getBetterAIMove(moveCoutner, board);
  int row = position / 3;
  int col = position % 3;
  strcpy(board[row][col], "O");
  moveCoutner++;
  player = 0;
  counterLastMove = 0;
  tone(buzzerPin, player1MoveNote, player1MoveNoteDuration);
}

void TicTacToeGame::getUserInputStanding() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    gameStatus = MENU;
    initializeBoard();
    moveCoutner = 0;
    player = randInt(0, 2);
    playWinDrawMelody = true;
    resetLights();
  }
  return;
}

void TicTacToeGame::mainLoopTic() {
  u8g.firstPage();
  do {
    if (gameStatus == MENU) {
      drawMenu();
      getUserInputMenu();
    } else if (gameStatus == GAMEPVSP || gameStatus == GAMEPVSAI) {
      drawBoard();
      boardDrawn = !boardDrawn;  // Set the flag after drawing the board
      if (checkWinnerTic(board) != ' ') {
        if (checkWinnerTic(board) == 'X') {
          gameStatus = STANDING_X;
          player1Wins++;
        } else {
          gameStatus = STANDING_O;
          player2Wins++;
        }
      }
      if (moveCoutner == 9) {
        gameStatus = DRAW;
      }
      if (gameStatus == GAMEPVSP) {
        getUserInputGame();
        boardDrawn = true;
      } else if (gameStatus == GAMEPVSAI) {
        if (player == 0) {
          getUserInputGame();
        } else {
          if (counterLastMove > 40) {
            getAIInput();
          }
          counterLastMove++;
        }
        boardDrawn = true;
      }
    } else if (gameStatus == STANDING_X || gameStatus == STANDING_O ||
               gameStatus == DRAW) {
      drawBoard();
      boardDrawn = true;
      getUserInputStanding();
    }
  } while (u8g.nextPage());

  // Play sounds only if the board has been drawn
  if (boardDrawn) {
    if (gameStatus == STANDING_O) {
      if (playWinDrawMelody) {
        digitalWrite(redLightPin, HIGH);
        playMusic(winMelody, winMelodyNoteDurations, winMelodySize);
        playWinDrawMelody = false;
      }
    } else if (gameStatus == STANDING_X) {
      if (playWinDrawMelody) {
        digitalWrite(greenLightPin, HIGH);
        playMusic(winMelody, winMelodyNoteDurations, winMelodySize);
        playWinDrawMelody = false;
      }
    } else if (gameStatus == DRAW) {
      if (playWinDrawMelody) {
        digitalWrite(yellowLightPin, HIGH);
        playMusic(drawMelody, drawMelodyNoteDurations, drawMelodySize);
        playWinDrawMelody = false;
      }
    }
    boardDrawn = false;  // Reset the flag after playing the sounds
  }
}
