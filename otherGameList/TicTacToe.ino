/*
TicTacToe.ino (MiniGames Project)

This file contains the code for the TicTacToe game.
It is a two player game where the players can play against each other.
The game is displayed on an OLED display and the players can use a
keypad to make their moves.
For different game outcomes, different melodies are played and different
lights are turned on.

More detailed information are in the file titled "ProjectDocumenation.pdf".

@author: Fabian Stiewe
@data: 10.11.2024

Used libraries:
- U8g2lib
- Keypad
Those are also inside the libaries folder, but should be installed
via the Arduino IDE.
*/

#include <Keypad.h>
#include <U8g2lib.h>

#include "Arduino.h"
#include "pitches.h"
using namespace std;

/**
 * #####################################################################
 *
 * Setup code for fixed variables
 *
 * #####################################################################
 */

/** From Oled instructions pdf
 * https://www.az-delivery.de/en/collections/kostenlose-e-books
 */
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

/**
 * Code and documentation for the keypad
 * https://www.az-delivery.de/en/products/4x4-matrix-keypad
 */
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

/**
 * Setups the pinModes for each light
 */
void setupPinModes() {
  pinMode(redLightPin, OUTPUT);
  pinMode(yellowLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(blueLightPin, OUTPUT);
}

void setup(void) {
  u8g.begin();
  setupPinModes();
  initializeBoard();
}

/**
 * #####################################################################
 *
 * Paste all game code below here
 *
 * #####################################################################
 */

typedef enum {
  MENU,
  STANDING_X,
  STANDING_O,
  DRAW,
  GAMEPVSP,
} GameStatus;

// Meldoies and notes for the game
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

int player = 1;
int player1Wins = 0;
int player2Wins = 0;
int moveCoutner = 0;
GameStatus gameStatus = MENU;
char board[3][3][2];

/**
 * Kinda complete variable
 * This is set and reset in the game loop
 * So that the board can be drawn with in one iteration
 */
bool boardDrawn = false;
bool setupGame = false;

int counterLastMove = 0;
int counterBoardDrawnFinal = 0;

/**
 * Checks if either X or O has won the game and returns the winner
 * @param board The current ttt board
 */
char checkWinnerTic(char board[3][3][2]) {
  for (int i = 0; i < 3; i++) {
    // check rows and columns
    if (board[i][0][0] == board[i][1][0] && board[i][1][0] == board[i][2][0]) {
      return board[i][0][0];
    }
    if (board[0][i][0] == board[1][i][0] && board[1][i][0] == board[2][i][0]) {
      return board[0][i][0];
    }
  }
  // check diagonals
  if (board[0][0][0] == board[1][1][0] && board[1][1][0] == board[2][2][0]) {
    return board[0][0][0];
  }
  // check other diagonal
  if (board[0][2][0] == board[1][1][0] && board[1][1][0] == board[2][0][0]) {
    return board[0][2][0];
  }
  return ' ';
}

/**
 * Play music function for given melody and note durations
 */
void playMusic(int *melody, int *noteDurations, int length) {
  for (int thisNote = 0; thisNote < length; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(buzzerPin);
  }
}

/**
 * Function to initialize the board
 */
void initializeBoard() {
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

/**
 * Function to draw the board on the OLED display
 */
void drawBoard() {
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
}

/**
 * Function to draw the player's turn on the OLED display
 */
void drawPlayersTurn() {
  if (player == 0) {
    u8g.drawStr(9, 62, "P1: X");
  } else {
    u8g.drawStr(9, 62, "P2: O");
  }
}

/**
 * Function to draw the game standings on the OLED display
 * P1: x   P2: y
 */
void drawGameStandings() {
  u8g.drawStr(60, 10, "Points:");
  u8g.drawStr(60, 22, "P1: ");
  u8g.drawStr(80, 22, String(player1Wins).c_str());
  u8g.drawStr(95, 22, "P2: ");
  u8g.drawStr(115, 22, String(player2Wins).c_str());
}

/**
 * Function to draw the game status on the OLED display
 */
void drawGameStatus() {
  if (gameStatus == STANDING_O) {
    u8g.drawStr(60, 62, "Player O wins!");
  } else if (gameStatus == STANDING_X) {
    u8g.drawStr(60, 40, "Player X wins!");
  } else if (gameStatus == DRAW) {
    u8g.drawStr(60, 62, "Draw!");
  }
}

/**
 * Function to draw the main display on the OLED
 */
void drawMainDisplay(void) {
  drawBoard();
  drawPlayersTurn();
  drawGameStandings();
  drawGameStatus();
}

/**
 * Function to draw the menu on the OLED display
 */
void drawMenu(void) {
  u8g.drawStr(0, 10, "Tic Tac Toe");
  u8g.drawStr(0, 20, "1: start");
}

/**
 * Function to reset the lights
 * (red, yellow, green are turned off)
 */
void resetLights() {
  digitalWrite(redLightPin, LOW);
  digitalWrite(yellowLightPin, LOW);
  digitalWrite(greenLightPin, LOW);
}

/**
 * Function to get the user input for the game
 * Calculates the row and col and updates the board
 * (if its a valid move)
 */
void getUserInputGame() {
  char key = keypad.getKey();
  if (key >= '1' && key <= '9') {
    int position = key - '1';
    int row = position / 3;
    int col = position % 3;
    if (strcmp(board[row][col], " ") == 0) {
      updateBoard(row, col);
      moveCoutner++;
      playPlayerMoveSound();
      return;
    }
  }
}

/**
 * Function to update the board with the player's move
 * @param row The row number of the board
 * @param col The column number of the board
 */
void updateBoard(int row, int col) {
  if (player == 0) {
    strcpy(board[row][col], "X");
    player = 1;
  } else {
    strcpy(board[row][col], "O");
    player = 0;
  }
}

/**
 * Function to play the sound of the player's move
 */
void playPlayerMoveSound() {
  if (player == 0) {
    tone(buzzerPin, player0MoveNote, player0MoveNoteDuration);
  } else {
    tone(buzzerPin, player1MoveNote, player1MoveNoteDuration);
  }
}

/**
 * Gets the user input for the menu
 */
void getUserInputMenu() {
  char key = keypad.getKey();
  if (key == '1') {
    gameStatus = GAMEPVSP;
    initializeBoard();
    return;
  }
}

/**
 * Gets the user input after the game
 * So the player can see the standings
 * Any key press will get him back to the
 * tic tac toe menu
 */
void getUserInputStanding() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    resetGame();
  }
  return;
}

/**
 * Resets the game, so a new round can be played
 * Standnings are obviously kept
 */
void resetGame() {
  gameStatus = MENU;
  initializeBoard();
  moveCoutner = 0;
  player = 1 - player;
  playWinDrawMelody = true;
  counterBoardDrawnFinal = 0;
  counterLastMove = 0;
  resetLights();
}

/**
 * Plays music if the game has ended
 */
void playGameEndMusic() {
  if (gameStatus == STANDING_O) {
    if (playWinDrawMelody) {
      digitalWrite(redLightPin, HIGH);
      playMusic(winMelody, winMelodyNoteDurations, winMelodySize);
    }
  } else if (gameStatus == STANDING_X) {
    if (playWinDrawMelody) {
      digitalWrite(greenLightPin, HIGH);
      playMusic(winMelody, winMelodyNoteDurations, winMelodySize);
    }
  } else {
    if (playWinDrawMelody) {
      digitalWrite(yellowLightPin, HIGH);
      playMusic(drawMelody, drawMelodyNoteDurations, drawMelodySize);
    }
  }
  playWinDrawMelody = false;
}

/**
 * Checks if one player has won
 */
void checkForWinner() {
  if (checkWinnerTic(board) != ' ') {
    if (checkWinnerTic(board) == 'X') {
      gameStatus = STANDING_X;
      player1Wins++;
    } else {
      gameStatus = STANDING_O;
      player2Wins++;
    }
  }
}

/**
 * Checks if the game is a draw
 * (all the cells are filled) and no one has won
 */
void checkForDraw() {
  if (moveCoutner == 9 && gameStatus != STANDING_X &&
      gameStatus != STANDING_O) {
    gameStatus = DRAW;
  }
}

/**
 * Checks if one player has won
 * or if the game is a draw
 */
void checkForGameEnding() {
  checkForWinner();
  checkForDraw();
}

/**
 * Loop for the end of the game
 * Show the standings, lights a led and plays music
 */
void loopEnd() {
  drawMainDisplay();
  boardDrawn = true;
  getUserInputStanding();
  if (counterBoardDrawnFinal > 50) {
    playGameEndMusic();
    counterBoardDrawnFinal = -32000;
  }
  counterBoardDrawnFinal++;
}

void loopGame() {
  drawMainDisplay();
  boardDrawn = !boardDrawn;  // Set the flag after drawing the board
  checkForGameEnding();
  getUserInputGame();
  boardDrawn = true;  // Set the flag after drawing the board
}

/**
 * Loop for the tictactoe menu
 */
void loopMenu() {
  drawMenu();
  getUserInputMenu();
}

void mainLoopTic() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g2_font_u8glib_4_hr);
    if (gameStatus == MENU) {
      loopMenu();
    } else if (gameStatus == GAMEPVSP) {
      loopGame();
    } else if (gameStatus == STANDING_X || gameStatus == STANDING_O ||
               gameStatus == DRAW) {
      loopEnd();
    }
  } while (u8g.nextPage());
}

/**
 * #####################################################################
 *
 * Call the game loop here
 *
 * #####################################################################
 */

void loop(void) { mainLoopTic(); }