#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#include "tic_tac_toe.h"

struct Player
{
    wchar_t name[256];
    wchar_t mark;
};

inline static bool is_win_index(const size_t index, const size_t win_indices[3])
{
    for (size_t j = 0; j < 3; ++j)
    {
        if (index == win_indices[j])
        {
            return true;
        }
    }

    return false;
} 

static void draw_number_row_check(const wchar_t symbols[static 9],
    const size_t start, const size_t win_indices[3])
{
    wprintf(L"│");

    for (size_t i = 0; i < 3; ++i)
    {
        const size_t current = start + i;
        if (is_win_index(current, win_indices))
        {
            wprintf(L"\033[9m"); // Crossed-out text.
            wprintf(L" %lc ", symbols[current]);
            wprintf(L"\033[0m│"); // Reset.
        }
        else
        {
            wprintf(L" %lc │", symbols[start + i]);
        }
    }

    wprintf(L"\n");
}

/**
 * @brief Draw current board state.
 *
 * @param symbols Symbols to display.
 * @param win_indices Indices representing victory state: -1s meaning no
 * victory.
 */
inline static void draw_board(
    const wchar_t symbols[static 9], const size_t win_indices[3])
{
    wprintf(L"┌───┬───┬───┐\n");
    draw_number_row_check(symbols, 0, win_indices);
    wprintf(L"├───┼───┼───┤\n");
    draw_number_row_check(symbols, 3, win_indices);
    wprintf(L"├───┼───┼───┤\n");
    draw_number_row_check(symbols, 6, win_indices);
    wprintf(L"└───┴───┴───┘\n");
}

/**
 * @brief Get the input from the current player.
 * @param symbols Current symbols present on the board.
 */
static void get_input(
    wchar_t symbols[static 9], const struct Player* const player)
{
    // Draw board to show the options.
    //draw_board(symbols);
    draw_board(symbols, (size_t[]){-1, -1, -1});

    // Ask the player to enter the cell number until a valid value is given.
    while (true)
    {
        wprintf(L"Player: %ls. Choose a cell number to place \'%lc\'.\n",
            player->name, player->mark);

        // Get the input as an integer value.
        size_t num = -1;
        // Decrement the value, as the indices in the array start from zero.
        if (wscanf(L"%zu", &num) && (--num < 9) && (symbols[num] >= '1') &&
            (symbols[num] <= '9'))
        {
            symbols[num] = player->mark;
            break;
        }
        else
        {
            wprintf(L"Cannot put the mark in the chosen cell.\n");
        }
    }
}

static bool check_triple(const wchar_t symbols[9], const size_t first,
    const size_t step, const wchar_t mark, size_t win_indices[3])
{
    const size_t second = first + step;
    const size_t third = second + step;
    if ((symbols[first] == mark) && (symbols[second] == mark) &&
        (symbols[third] == mark))
    {
        win_indices[0] = first;
        win_indices[1] = second;
        win_indices[2] = third;

        return true;
    }
    else
    {
        return false;
    }
}

static bool check_rows(
    const wchar_t symbols[9], const wchar_t mark, size_t win_indices[3])
{
    for (size_t i = 0; i < 3; ++i)
    {
        if (check_triple(symbols, i * 3, 1, mark, win_indices))
        {
            return true;
        }
    }

    return false;
}

static bool check_columns(
    const wchar_t symbols[9], const wchar_t mark, size_t win_indices[3])
{
    for (size_t i = 0; i < 3; ++i)
    {
        if (check_triple(symbols, i, 3, mark, win_indices))
        {
            return true;
        }
    }

    return false;
}

static bool check_diagonals(
    const wchar_t symbols[9], const wchar_t mark, size_t win_indices[3])
{

    if (check_triple(symbols, 0, 4, mark, win_indices))
    {
        return true;
    }
    else if (check_triple(symbols, 2, 2, mark, win_indices))
    {
        return true;
    }
    else
    {
        return false;
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
static bool check_victory(
    const wchar_t symbols[9], const struct Player* const player)
{
    size_t win_indices[3] = {-1, -1, -1};

    if (check_rows(symbols, player->mark, win_indices) ||
        check_columns(symbols, player->mark, win_indices) ||
        check_diagonals(symbols, player->mark, win_indices))
    {
        draw_board(symbols, win_indices);
        wprintf(L"Player %ls won!\n", player->name);

        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Get player's input and check if the game ends.
 *
 * @param symbols Current symbols present on the board.
 * @param player Current player.
 * @return true Player wins the game.
 * @return false Game continues.
 */
inline static bool victorious_turn(
    wchar_t symbols[9], const struct Player* const player)
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
static size_t game_loop(wchar_t symbols[static 9],
    const struct Player* const first_player_ptr,
    const struct Player* const second_player_ptr)
{
    bool first_player_turn = true;

    const struct Player* current_player_ptr = first_player_ptr;

    size_t turn_count = 0;

    // Using fixed number, as in tic-tac-toe there are always 9 cells.
    while (++turn_count <= 9)
    {
        if (victorious_turn(symbols, current_player_ptr))
        {
            break;
        }

        // Switch player's turn af the correct input was given and if the game
        // goes on.
        if (first_player_turn)
        {
            current_player_ptr = second_player_ptr;
            first_player_turn = false;
        }
        else
        {
            current_player_ptr = first_player_ptr;
            first_player_turn = true;
        }
    }

    return turn_count;
}

void run_game(void)
{
    setlocale(LC_ALL, "");

    wchar_t symbols[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    struct Player player1 = {.name = L"John", .mark = 'X'};
    struct Player player2 = {.name = L"Mary", .mark = 'O'};

    size_t turn_count = game_loop(symbols, &player1, &player2);

    if (turn_count == 9)
    {
        // Draw the board to display the final state of the game.
        // draw_board(symbols);
        draw_board(symbols, (size_t[]){-1, -1, -1});

        wprintf(L"No winner, it\'s a draw.\n");
    }
}
