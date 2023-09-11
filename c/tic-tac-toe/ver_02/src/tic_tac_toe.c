#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#include "tic_tac_toe.h"

struct Player
{
    wchar_t name[256];
    char mark;
};

static void draw_top_row()
{
#if 1
    wprintf(L"┌───┬───┬───┐\n");
#else
    printf(
        TOP_LEFT HORIZONTAL_3 T_DOWN HORIZONTAL_3 T_DOWN HORIZONTAL_3 TOP_RIGHT
        "\n");
#endif
}

static void draw_middle_row()
{
#if 1
    wprintf(L"├───┼───┼───┤\n");
#else
    printf(T_RIGHT HORIZONTAL_3 INTERSECTION HORIZONTAL_3 INTERSECTION
            HORIZONTAL_3 T_LEFT "\n");
#endif
}

static void draw_bottom_row()
{
#if 1
    wprintf(L"└───┴───┴───┘\n");
#else
    printf(BOTTOM_LEFT HORIZONTAL_3 T_UP HORIZONTAL_3 T_UP HORIZONTAL_3
            BOTTOM_RIGHT "\n");
#endif
}

static void draw_number_row(const char symbols[static 9], const size_t start)
{
#if 1
    wprintf(L"│ %Lc │ %Lc │ %Lc │\n",
        symbols[start], symbols[start + 1], symbols[start + 2]);
#else
    printf(VERTICAL " %c " VERTICAL " %c " VERTICAL " %c " VERTICAL "\n",
        symbols[start], symbols[start + 1], symbols[start + 2]);
#endif
}

static void draw_number_row_check(const char symbols[static 9],
    const size_t start, const size_t win_indices[3])
{
    printf(VERTICAL);

    for (size_t i = 0; i < 3; ++i)
    {
        bool found = false;
        const size_t current = start + i;
        for (size_t j = 0; j < 3; ++j)
        {
            if (current == win_indices[j])
            {
                printf("\033[9m"); // Crossed-out text.
                printf(" %c ", symbols[current]);
                printf("\033[0m"); // Reset.

                found = true;

                break;
            }
        }

        if (!found)
        {
            printf(" %c ", symbols[start + i]);
        }
        printf(VERTICAL);
    }

    printf("\n");
}

/**
 * @brief Display current state of the tic-tac-toe board
 * @param symbols Current symbols present on the board.
 */
void draw_board(const char symbols[static 9])
{
    // printf("\033[9m"); // Crossed-out text.

    draw_top_row();
    draw_number_row(symbols, 0);
    draw_middle_row();
    draw_number_row(symbols, 3);
    draw_middle_row();
    draw_number_row(symbols, 6);
    draw_bottom_row();
}

static void draw_board_check(
    const char symbols[static 9], const size_t win_indices[3])
{
    draw_top_row();
    draw_number_row_check(symbols, 0, win_indices);
    draw_middle_row();
    draw_number_row_check(symbols, 3, win_indices);
    draw_middle_row();
    draw_number_row_check(symbols, 6, win_indices);
    draw_bottom_row();
}

/**
 * @brief Get the input from the current player.
 * @param symbols Current symbols present on the board.
 */
static void get_input(char symbols[static 9], const struct Player player)
{
    // Draw board to show the options.
    draw_board(symbols);

    // Ask the player to enter the cell number until a valid value is given.
    while (true)
    {
        wprintf(L"Player: %Ls. Choose a cell number to place \'%Lc\'.\n",
            player.name, player.mark);

        // Get the input as an integer value.
        size_t num = -1;
        // Decrement the value, as the indices in the array start from zero.
        if (wscanf(L"%zu", &num) && (num > 0) && (--num < 9) &&
            (symbols[num] >= '1') && (symbols[num] <= '9'))
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
static bool check_victory(const char* const symbols, const struct Player player)
{
    bool victory = false;

    const char mark = player.mark;

    size_t win_indices[3] = {-1, -1, -1};

    // Check rows.
    for (size_t i = 0; i < 9; i += 3)
    {
        if ((symbols[i] == mark) && (symbols[i + 1] == mark) &&
            (symbols[i + 2] == mark))
        {
            for (size_t j = 0; j < 3; ++j)
            {
                win_indices[j] = i + j;
            }

            victory = true;
            break;
        }
    }

    // Check columns.
    for (size_t i = 0; i < 3; ++i)
    {
        if ((symbols[i] == mark) && (symbols[i + 3] == mark) &&
            (symbols[i + 6] == mark))
        {
            for (size_t j = 0; j < 3; ++j)
            {
                win_indices[j] = i + j * 3;
            }

            victory = true;
            break;
        }
    }

    if ((symbols[0] == mark) && (symbols[4] == mark) && (symbols[8] == mark))
    {
        for (size_t j = 0; j < 3; ++j)
        {
            win_indices[j] = j * 4;
        }

        victory = true;
    }
    else if ((symbols[2] == mark) && (symbols[4] == mark) &&
             (symbols[6] == mark))
    {
        for (size_t j = 0; j < 3; ++j)
        {
            win_indices[j] = 2 + (j << 1);
        }

        victory = true;
    }

    if (victory)
    {
        draw_board_check(symbols, win_indices);
        // Inform about the winner.
        wprintf(L"Player %Ls has won!\n", player.name);
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
static bool victorious_turn(char* const symbols, const struct Player player)
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
static size_t game_loop(char symbols[static 9], const struct Player player1,
    const struct Player player2)
{
    bool first_player_turn = true;

    size_t turn_count = 0;

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
    setlocale(LC_ALL, "");

    char symbols[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    struct Player player1 = {.name = L"John", .mark = 'X'};
    struct Player player2 = {.name = L"Mary", .mark = 'O'};

    size_t turn_count = game_loop(symbols, player1, player2);

    if (turn_count == 9)
    {
        // Draw the board to display the final state of the game.
        draw_board(symbols);

        wprintf(L"No winner, it\'s a draw.\n");
    }
}
