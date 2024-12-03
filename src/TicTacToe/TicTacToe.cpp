#include "TicTacToe.h"

#include <Arduino.h>

#include "../utils/utils.h"

using namespace std;

/**
 * Checks if either X or O has won the game and returns the winner
 * If neither has won, it returns a space
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
 * Function to get the best move for the AI
 *
 * The AI is symbolized by 'O' and the player by 'X'
 *
 * Returns a number between 0 and 8 representing the best move
 * 0 - 8 maps (bijektiv) to 1 - 9 on the keypad
 *
 * @param moveCounter The number of moves that have been made
 * @param board The current board
 */
int getBetterAIMove(int moveCounter, char board[3][3][2]) {
  // Start move into the middle of the fields
  if (moveCounter == 0) {
    return 4;
  }

  // Second move: If the middle is free, take it; otherwise, take a corner
  if (moveCounter == 1) {
    if (board[1][1][0] == ' ') {
      return 4;
    } else {
      return 0;
    }
  }

  // Try to place in one of the corners
  if (moveCounter == 2) {
    if (board[0][0][0] == ' ') return 0;
    if (board[0][2][0] == ' ') return 2;
    if (board[2][0][0] == ' ') return 6;
    if (board[2][2][0] == ' ') return 8;
  }

  // Check if AI can win in one move
  int winMove = checkPossibleOneMoveWin(board, 'O');
  if (winMove != -1) {
    return winMove;
  }

  // Check if the player can win in one move and block them
  winMove = checkPossibleOneMoveWin(board, 'X');
  if (winMove != -1) {
    return winMove;
  }

  // Replace std::vector with a fixed-size array
  int possibleMoves[9];
  int moveCount = 0;

  // Collect all possible moves
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j][0] == ' ') {
        possibleMoves[moveCount++] = i * 3 + j;
      }
    }
  }

  // Choose a random move
  if (moveCount > 0) {
    return possibleMoves[randInt(0, moveCount - 1)];
  }

  return -1;  // No valid moves
}

/**
 * Checks if there is a possible move to win in one move
 * @param board_copy The current board
 * @param player The player to check for
 */
int checkPossibleOneMoveWin(char board_copy[3][3][2], char player) {
  // check the rows
  for (int i = 0; i < 3; i++) {
    if (board_copy[i][0][0] == board_copy[i][1][0] &&
        board_copy[i][0][0] == player && board_copy[i][2][0] == ' ') {
      return 3 * i + 3 - 1;
    }
    if (board_copy[i][1][0] == board_copy[i][2][0] &&
        board_copy[i][1][0] == player && board_copy[i][0][0] == ' ') {
      return 3 * i + 1 - 1;
    }
    if (board_copy[i][0][0] == board_copy[i][2][0] &&
        board_copy[i][0][0] == player && board_copy[i][1][0] == ' ') {
      return 3 * i + 2 - 1;
    }
  }

  // check the columns
  for (int i = 0; i < 3; i++) {
    if (board_copy[0][i][0] == board_copy[1][i][0] &&
        board_copy[0][i][0] == player && board_copy[2][i][0] == ' ') {
      return 7 + i - 1;
    }
    if (board_copy[1][i][0] == board_copy[2][i][0] &&
        board_copy[1][i][0] == player && board_copy[0][i][0] == ' ') {
      return 1 + i - 1;
    }
    if (board_copy[0][i][0] == board_copy[2][i][0] &&
        board_copy[0][i][0] == player && board_copy[1][i][0] == ' ') {
      return 4 + i - 1;
    }
  }

  // check the diagonals
  if (board_copy[0][0][0] == board_copy[1][1][0] &&
      board_copy[0][0][0] == player && board_copy[2][2][0] == ' ') {
    return 9 - 1;
  }
  if (board_copy[1][1][0] == board_copy[2][2][0] &&
      board_copy[1][1][0] == player && board_copy[0][0][0] == ' ') {
    return 1 - 1;
  }
  if (board_copy[0][0][0] == board_copy[2][2][0] &&
      board_copy[0][0][0] == player && board_copy[1][1][0] == ' ') {
    return 5 - 1;
  }
  if (board_copy[0][2][0] == board_copy[1][1][0] &&
      board_copy[0][2][0] == player && board_copy[2][0][0] == ' ') {
    return 7 - 1;
  }
  if (board_copy[1][1][0] == board_copy[2][0][0] &&
      board_copy[1][1][0] == player && board_copy[0][2][0] == ' ') {
    return 3 - 1;
  }
  if (board_copy[0][2][0] == board_copy[2][0][0] &&
      board_copy[0][2][0] == player && board_copy[1][1][0] == ' ') {
    return 5 - 1;
  }

  // not possible to win in one move
  return -1;
}

/**
 * The MinMax algorithm to get the best move for the AI
 * @param round The current round
 * @param board_copy The current board
 * @param player_here who's turn it is
 *
 */
Pair getAIinputMinMaxHelper(int round, char board_copy[3][3][2],
                            bool player_here) {
  // If the board is full, it's a draw
  if (round == 9) {
    return {0, -1};
  }

  // Collect all free fields
  int free_fields[9];
  int fieldCount = 0;

  for (int i = 0; i < 9; i++) {
    if (board_copy[i / 3][i % 3][0] == ' ') {
      free_fields[fieldCount++] = i;
    }
  }

  // Save all the results
  int results[9];
  int resultCount = 0;

  for (int i = 0; i < fieldCount; i++) {
    int free_field = free_fields[i];
    char board_copy_move[3][3][2];

    // Copy the board
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        board_copy_move[i][j][0] = board_copy[i][j][0];
        board_copy_move[i][j][1] = board_copy[i][j][1];
      }
    }

    // Simulate a move
    board_copy_move[free_field / 3][free_field % 3][0] =
        player_here ? 'X' : 'O';

    // Check for a win
    char winner = checkWinnerTic(board_copy_move);
    if (winner == 'X') return {-1, -1};
    if (winner == 'O') return {1, free_field};

    // Recur for the opponent
    Pair tmp = getAIinputMinMaxHelper(round + 1, board_copy_move, !player_here);
    results[resultCount++] = tmp.first;
  }

  // Find the best move
  int bestResult = player_here ? 2 : -2;
  int move = -1;

  for (int i = 0; i < resultCount; ++i) {
    int result = results[i];
    if ((player_here && result < bestResult) ||
        (!player_here && result > bestResult)) {
      bestResult = result;
      move = free_fields[i];
    }
  }

  return {bestResult, move};
}