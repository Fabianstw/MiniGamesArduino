#include <vector>
#include <iostream>
#include <utility>

#include "TicTacToe.h"
#include "../utils/utils.h"

using namespace std;

char checkWinnerTic(char board[3][3][2])
{
    for (int i = 0; i < 3; i++)
    {
        // check rows and columns
        if (board[i][0][0] == board[i][1][0] && board[i][1][0] == board[i][2][0])
        {
            return board[i][0][0];
        }
        if (board[0][i][0] == board[1][i][0] && board[1][i][0] == board[2][i][0])
        {
            return board[0][i][0];
        }
    }
    // check diagonals
    if (board[0][0][0] == board[1][1][0] && board[1][1][0] == board[2][2][0])
    {
        return board[0][0][0];
    }
    // check other diagonal
    if (board[0][2][0] == board[1][1][0] && board[1][1][0] == board[2][0][0])
    {
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
int getBetterAIMove(int moveCounter, char board[3][3][2])
{
    // Start move into the middle of the fields
    if (moveCounter == 0)
    {
        return 4;
    }

    // Secound move:
    // If the middle is free, take it
    // If not, take a top left corner TODO: change to random corner
    if (moveCounter == 1)
    {
        if (board[1][1][0] == ' ')
        {
            return 4;
        }
        else
        {
            return 0;
        }
    }

    // check if AI can win in one move
    int winMove = checkPossibleOneMoveWin(board, 'O');
    if (winMove != -1)
    {
        return winMove;
    }

    // check if player can win in one move
    // if yes, block the player
    winMove = checkPossibleOneMoveWin(board, 'X');
    if (winMove != -1)
    {
        return winMove;
    }

    pair<int, int> answer = getAIinputMinMax(moveCounter, board, false);
    if (0 <= answer.second && answer.second <= 8)
    {
        return answer.second;
    }

    // get all possible moves
    vector<int> possibleMoves;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j][0] == ' ')
            {
                possibleMoves.push_back(i * 3 + j);
            }
        }
    }
    // choose a random move
    return possibleMoves[randInt(0, possibleMoves.size() - 1)];
}

int checkPossibleOneMoveWin(char board_copy[3][3][2], char player)
{
    // check the rows
    for (int i = 0; i < 3; i++)
    {
        if (board_copy[i][0][0] == board_copy[i][1][0] && board_copy[i][0][0] == player && board_copy[i][2][0] == ' ')
        {
            return 3 * i + 3 - 1;
        }
        if (board_copy[i][1][0] == board_copy[i][2][0] && board_copy[i][1][0] == player && board_copy[i][0][0] == ' ')
        {
            return 3 * i + 1 - 1;
        }
        if (board_copy[i][0][0] == board_copy[i][2][0] && board_copy[i][0][0] == player && board_copy[i][1][0] == ' ')
        {
            return 3 * i + 2 - 1;
        }
    }

    // check the columns
    for (int i = 0; i < 3; i++)
    {
        if (board_copy[0][i][0] == board_copy[1][i][0] && board_copy[0][i][0] == player && board_copy[2][i][0] == ' ')
        {
            return 7 + i - 1;
        }
        if (board_copy[1][i][0] == board_copy[2][i][0] && board_copy[1][i][0] == player && board_copy[0][i][0] == ' ')
        {
            return 1 + i - 1;
        }
        if (board_copy[0][i][0] == board_copy[2][i][0] && board_copy[0][i][0] == player && board_copy[1][i][0] == ' ')
        {
            return 4 + i - 1;
        }
    }

    // check the diagonals
    if (board_copy[0][0][0] == board_copy[1][1][0] && board_copy[0][0][0] == player && board_copy[2][2][0] == ' ')
    {
        return 9 - 1;
    }
    if (board_copy[1][1][0] == board_copy[2][2][0] && board_copy[1][1][0] == player && board_copy[0][0][0] == ' ')
    {
        return 1 - 1;
    }
    if (board_copy[0][0][0] == board_copy[2][2][0] && board_copy[0][0][0] == player && board_copy[1][1][0] == ' ')
    {
        return 5 - 1;
    }
    if (board_copy[0][2][0] == board_copy[1][1][0] && board_copy[0][2][0] == player && board_copy[2][0][0] == ' ')
    {
        return 7 - 1;
    }
    if (board_copy[1][1][0] == board_copy[2][0][0] && board_copy[1][1][0] == player && board_copy[0][2][0] == ' ')
    {
        return 3 - 1;
    }
    if (board_copy[0][2][0] == board_copy[2][0][0] && board_copy[0][2][0] == player && board_copy[1][1][0] == ' ')
    {
        return 5 - 1;
    }

    // not possible to win in one move
    return -1;
}

pair<int, int> getAIinputMinMax(int round, char board_copy[3][3][2], bool player_here)
{
    // Calculate all possible games till the end
    // win is 1, draw is 0, lose is -1
    if (round == 9)
    {
        return make_pair(0, -1); // Draw if no winner and board is full
    }

    // Get all free fields
    vector<int> free_fields;
    for (int i = 0; i < 9; i++)
    {
        if (board_copy[i / 3][i % 3][0] == ' ')
        {
            free_fields.push_back(i);
        }
    }

    // Save all the results
    vector<int> results;
    for (int free_field : free_fields)
    {
        char board_copy_move[3][3][2];
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                board_copy_move[i][j][0] = board_copy[i][j][0];
                board_copy_move[i][j][1] = board_copy[i][j][1];
            }
        }

        // Simulate a move
        board_copy_move[free_field / 3][free_field % 3][0] = player_here ? 'X' : 'O';

        // Check for a win
        char winner = checkWinnerTic(board_copy_move);
        if (winner == 'X')
            return make_pair(-1, -1);
        if (winner == 'O')
            return make_pair(1, free_field);

        // Recur for the opponent
        pair<int, int> tmp = getAIinputMinMax(round + 1, board_copy_move, !player_here);
        results.push_back(tmp.first);
    }

    int bestResult = player_here ? 2 : -2;
    int move = -1;
    for (size_t i = 0; i < results.size(); ++i)
    {
        int result = results[i];
        int moveIndex = free_fields[i];
        if ((player_here && result < bestResult) || (!player_here && result > bestResult))
        {
            bestResult = result;
            move = moveIndex; // Corrected without adding 1
        }
    }

    return make_pair(bestResult, move);
}