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

/*
For different sound effects, we can use the following melodies:
*/
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
 * Constructor for the TicTacToeGame class
 * @param buzzerPin The pin number for the buzzer
 * @param redLightPin The pin number for the red light
 * @param yellowLightPin The pin number for the yellow light
 * @param greenLightPin The pin number for the green light
 * @param u8g The U8GLIB_SSD1306_128X64 object for the OLED display
 * @param keypad The Keypad object
 */
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

/**
 * Function to setup the TicTacToe game
 */
void TicTacToeGame::setupTicTacToe() {
  initializeBoard();
  // playMusic(startingGame, startingGameNoteDurations, startingGameSize);
}

/**
 * Play music function for given melody and note durations
 */
void TicTacToeGame::playMusic(int *melody, int *noteDurations, int length) {
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

/**
 * Function to draw the board on the OLED display
 */
void TicTacToeGame::drawBoard() {
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
}

/**
 * Function to draw the player's turn on the OLED display
 */
void TicTacToeGame::drawPlayersTurn() {
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
void TicTacToeGame::drawGameStandings() {
  u8g.drawStr(60, 10, "Points:");
  u8g.drawStr(60, 22, "P1: ");
  u8g.drawStr(80, 22, String(player1Wins).c_str());
  u8g.drawStr(95, 22, "P2: ");
  u8g.drawStr(115, 22, String(player2Wins).c_str());
}

/**
 * Function to draw the game status on the OLED display
 */
void TicTacToeGame::drawGameStatus() {
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
 * - Board
 * - Player's turn
 * - Game standings
 * - Game status
 */
void TicTacToeGame::drawMainDisplay(void) {
  drawBoard();
  drawPlayersTurn();
  drawGameStandings();
  drawGameStatus();
}

/**
 * Function to draw the menu on the OLED display
 * - Player vs Player
 * - Player vs AI
 * - Back to menu
 */
void TicTacToeGame::drawMenu(void) {
  u8g.drawStr(0, 10, "Tic Tac Toe");
  u8g.drawStr(0, 20, "1: P vs P");
  u8g.drawStr(0, 30, "2: P vs AI");

  u8g.drawStr(0, 50, "3: Back to menu");
}

/**
 * Function to reset the lights
 * (red, yellow, green are turned off)
 */
void TicTacToeGame::resetLights() {
  digitalWrite(redLightPin, LOW);
  digitalWrite(yellowLightPin, LOW);
  digitalWrite(greenLightPin, LOW);
}

/**
 * Function to get the user input for the game
 * Calculates the row and col and updates the board
 * (if its a valid move)
 */
void TicTacToeGame::getUserInputGame() {
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
void TicTacToeGame::updateBoard(int row, int col) {
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
void TicTacToeGame::playPlayerMoveSound() {
  if (player == 0) {
    tone(buzzerPin, player0MoveNote, player0MoveNoteDuration);
  } else {
    tone(buzzerPin, player1MoveNote, player1MoveNoteDuration);
  }
}

/**
 * Gets the user input for the menu
 * - Player vs Player
 * - Player vs AI
 * - Back to menu
 */
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

/**
 * Gets the AI input, places it on the board
 */
void TicTacToeGame::getAIInput() {
  int position = getBetterAIMove(moveCoutner, board);
  // AI only returns valid moves
  int row = position / 3;
  int col = position % 3;
  strcpy(board[row][col], "O");
  moveCoutner++;
  player = 0;
  counterLastMove = 0;
  playPlayerMoveSound();
}

/**
 * Gets the user input after the game
 * So the player can see the standings
 * Any key press will get him back to the
 * tic tac toe menu
 */
void TicTacToeGame::getUserInputStanding() {
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
void TicTacToeGame::resetGame() {
  gameStatus = MENU;
  initializeBoard();
  moveCoutner = 0;
  player = randInt(0, 2);
  playWinDrawMelody = true;
  counterBoardDrawnFinal = 0;
  counterLastMove = 0;
  resetLights();
}

/**
 * Plays music if the game has ended
 */
void TicTacToeGame::playGameEndMusic() {
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
void TicTacToeGame::checkForWinner() {
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
void TicTacToeGame::checkForDraw() {
  if (moveCoutner == 9 && gameStatus != STANDING_X &&
      gameStatus != STANDING_O) {
    gameStatus = DRAW;
  }
}

/**
 * Checks if one player has won
 * or if the game is a draw
 */
void TicTacToeGame::checkForGameEnding() {
  checkForWinner();
  checkForDraw();
}

/**
 * Loop for the end of the game
 * Show the standings, lights a led and plays music
 */
void TicTacToeGame::loopEnd() {
  drawMainDisplay();
  boardDrawn = true;
  getUserInputStanding();
  if (counterBoardDrawnFinal > 50) {
    playGameEndMusic();
    counterBoardDrawnFinal = -32000;
  }
  counterBoardDrawnFinal++;
}

void TicTacToeGame::loopGame() {
  drawMainDisplay();
  boardDrawn = !boardDrawn;  // Set the flag after drawing the board
  checkForGameEnding();
  if (gameStatus == GAMEPVSP) {
    getUserInputGame();
  } else if (gameStatus == GAMEPVSAI) {
    if (player == 0) {
      getUserInputGame();
    } else {
      if (counterLastMove > 50) {
        getAIInput();
        counterLastMove = 0;
      }
      counterLastMove++;
    }
  }
  boardDrawn = true;  // Set the flag after drawing the board
}

/**
 * Loop for the tictactoe menu
 */
void TicTacToeGame::loopMenu() {
  drawMenu();
  getUserInputMenu();
}

void TicTacToeGame::mainLoopTic() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_tpss);
    if (gameStatus == MENU) {
      loopMenu();
    } else if (gameStatus == GAMEPVSP || gameStatus == GAMEPVSAI) {
      loopGame();
    } else if (gameStatus == STANDING_X || gameStatus == STANDING_O ||
               gameStatus == DRAW) {
      loopEnd();
    }
  } while (u8g.nextPage());
}