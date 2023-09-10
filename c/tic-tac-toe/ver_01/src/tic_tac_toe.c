#include <stdio.h>
#include <stdbool.h>

#include "tic_tac_toe.h"

struct Player
{
    char name[256];
    char mark;
};

/**
 * @brief Display current state of the tic-tac-toe board
 * @param symbols Current symbols present on the board.
 */
void draw_board(const char* const symbols)
{
    printf("-------\n");
    printf("|%c|%c|%c|\n", symbols[0], symbols[1], symbols[2]);
    printf("-------\n");
    printf("|%c|%c|%c|\n", symbols[3], symbols[4], symbols[5]);
    printf("-------\n");
    printf("|%c|%c|%c|\n", symbols[6], symbols[7], symbols[8]);
    printf("-------\n");
}

/**
 * @brief Get the input from the current player.
 * @param symbols Current symbols present on the board.
 */
void get_input(char* const symbols, const struct Player player)
{
    // Draw board to show the options.
    draw_board(symbols);

    // Ask the player to enter the cell number until a valid value is given.
    while (true)
    {
        printf("Player: %s. Choose a cell number to place \'%c\'.\n",
            player.name, player.mark);

        // Get the input as an integer value.
        int num;
        scanf("%d", &num);

        // Reduce the value, as the indices in the array start from zero.
        --num;

        if ((num >= 0) && (num < 9) && (symbols[num] >= '1') &&
            (symbols[num] <= '9'))
        {
            symbols[num] = player.mark;
            break;
        }
        else
        {
            printf("Cannot put the mark in the chosen cell.\n");
        }
    }
}

/**
 * @brief Check if the player's put mark ends the game.
 * 
 * @param symbols Current symbols present on the board.
 * @param player Current player.
 * @return true Player wins the game.
 * @return false Game continues.
 */
bool check_victory(const char* const symbols, const struct Player player)
{
    bool victory = false;

    const char mark = player.mark;

    // Check rows.
    for (int i = 0; i < 9; i += 3)
    {
        if ((symbols[i] == mark) && (symbols[i + 1] == mark) &&
            (symbols[i + 2] == mark))
        {
            victory = true;
            break;
        }
    }

    // Check columns.
    for (int i = 0; i < 3; ++i)
    {
        if ((symbols[i] == mark) && (symbols[i + 3] == mark) &&
            (symbols[i + 6] == mark))
        {
            victory = true;
            break;
        }
    }

    // Check diagonals.
    if ((symbols[4] == mark) &&
        (((symbols[0] == mark) && (symbols[8] == mark)) ||
            ((symbols[2] == mark) && (symbols[6] == mark))))
    {
        victory = true;
    }

    if (victory)
    {
        // Inform about the winner.
        printf("Player %s has won!\n", player.name);
    }

    return victory;
}

/**
 * @brief Get player's input and check if the game ends.
 * 
 * @param symbols Current symbols present on the board.
 * @param player Current player.
 * @return true Player wins the game.
 * @return false Game continues.
 */
bool victorious_turn(char* const symbols, const struct Player player)
{
    get_input(symbols, player);

    return check_victory(symbols, player);
}

/**
 * @brief Main loop of the game.
 *
 * @param symbols Board symbols.
 * @param player1 First player.
 * @param player2 Second player.
 *
 * @return int Number of turns made during the game.
 */
int game_loop(char* const symbols, const struct Player player1,
    const struct Player player2)
{
    bool first_player_turn = true;

    int turn_count = 0;

    // Using fixed number, as in tic-tac-toe there are always 9 cells.
    while (turn_count < 9)
    {
        // Switch player's turn af the correct input was given and if the game
        // goes on.
        if (first_player_turn)
        {
            if (victorious_turn(symbols, player1))
            {
                break;
            }

            first_player_turn = false;
        }
        else
        {
            if (victorious_turn(symbols, player2))
            {
                break;
            }

            first_player_turn = true;
        }

        ++turn_count;
    }

    return turn_count;
}

void run_game(void)
{
    char symbols[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    struct Player player1 = {.name = "John", .mark = 'X'};
    struct Player player2 = {.name = "Mary", .mark = 'O'};

    int turn_count = game_loop(symbols, player1, player2);

    if (turn_count == 9)
    {
        printf("No winner, it\'s a draw.\n");
    }

    // Draw the board to display the final state of the game.
    draw_board(symbols);
}
