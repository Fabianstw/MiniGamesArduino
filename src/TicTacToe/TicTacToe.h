#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <utility>
using std::pair;

char checkWinnerTic(char board[3][3][2]);
int getBetterAIMove(int moveCoutner, char board[3][3][2]);
int checkPossibleOneMoveWin(char board_copy[3][3][2], char player);
pair<int, int> getAIinputMinMax(int round, char board_copy[3][3][2], bool player_here);

#endif // TICTACTOE_H