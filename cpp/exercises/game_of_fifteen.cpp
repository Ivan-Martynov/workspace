/*
Board things:
    - Dispaly the game board;
    - Display an invidiual tile;
    - Randomize the starting state;
    - Slide tiles;
    - Determine if the win condition reached.

User things:
    - Get commands from the user;
    - Handle invalid input;
    - Allow user to quit before winning.

class Board:
    Members:
        - 2D array of tiles.
    Methods:
    - Display board;
    - Randomize starting state (shuffle);
    - Slide tiles;
    - Determine if the win condition reached.
The game is 4x4 grid of tiles. The main purpose of this class is to manage the
2D array of tiles. The class is also responsible for randomizing, moving tiles,
displayign the board and checking whether the puzzle was solved.

class Tile:
    Members:
        - display number.
    Methods:
        - Display an invidiual tile;
This class represents a single tile withing the game board. Using a class allows
to overload the output operator to display the tile using desired format. Also
allows to have well-named methods to increase the readability of code related to
a single tile.

namespace UserInput:
    - Get commands from user;
    - Handle invalid input.
This namespace has function to manage the user input. It doesn't have any state,
therefore no need for a class.

function main:
    - Game loop;
    - Allow user to quit before winning.
Place to write the main game loop. Handle board game setup, coordinate
retrieving user inputs and command processing, as well as handle exit
conditions (i.e., win or quit).

Helper classes:

    - Point: for indexing tiles.
        Members:
            - x and y coordinates.

    - Direction: the user enters single-letter commands to slide tiles in
cardinal directions (e.g., 'w' = up, 'a' = left). Converting these commands into
a Direction object makes code more intuitive.
        Members:
            - Enum direction.

*/

#include "Input.h"
#include "Random.h"

#include <iostream>
#include <array>
#include <algorithm>
#include <cassert>

constexpr int g_console_lines {40};

class Direction
{
  public:
    enum Type
    {
        up,
        down,
        left,
        right,

        max_moves,
    };

    constexpr Direction(Type move_direction) : m_type {move_direction} {}

    constexpr Type get_type() const
    {
        return m_type;
    }

    Direction operator-() const
    {
        switch (m_type)
        {
            case up:
                return down;
            case down:
                return up;
            case left:
                return right;
            case right:
                return left;
            default:
                assert(false && "Unsupported direction was passed.");
                return up;
        }
    }

    static Direction random_direction()
    {
        return m_all_moves[Random::get(0, max_moves - 1)];
    }

    friend std::ostream& operator<<(std::ostream& stream, const Direction& dir);

  private:
    Type m_type;

    static constexpr std::array<Type, max_moves> m_all_moves {
        up, down, left, right};
    static constexpr std::array<std::string_view, max_moves> m_move_names {
        "up", "down", "left", "right"};
};

std::ostream& operator<<(std::ostream& stream, const Direction& dir)
{
    stream << Direction::m_move_names[dir.get_type()];
    return stream;
}

struct Point
{
    int x;
    int y;

    bool operator==(const Point& other) const
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(const Point& other) const
    {
        return !(*this == other);
    }

    Point get_adjacent_point(Direction direction) const
    {
        switch (direction.get_type())
        {
            case Direction::left:
                return Point {x - 1, y};
            case Direction::right:
                return Point {x + 1, y};
            case Direction::down:
                return Point {x, y + 1};
            case Direction::up:
                return Point {x, y - 1};
            default:
                assert(false && "Unsupported direction was passed.");
                return *this;
        }
    }
};

class Tile
{
  public:
    explicit constexpr Tile() = default;
    explicit constexpr Tile(int index) : m_index {index} {}

    constexpr int get_num() const
    {
        return m_index;
    }

    constexpr bool is_empty() const
    {
        return get_num() == 0;
    }

  private:
    int m_index {0};
};

std::ostream& operator<<(std::ostream& stream, const Tile& tile)
{
    stream << " ";

    if (!tile.is_empty())
    {
        if (tile.get_num() < 10)
        {
            std::cout << " ";
        }
        stream << tile.get_num();
    }
    else
    {
        stream << "  ";
    }

    stream << " ";

    return stream;
}

class Board
{
  private:
    static constexpr int m_size {4};
    Tile m_tiles[m_size][m_size];

  public:
    Board()
    {
        int count {0};
        for (int i {0}; i < m_size; ++i)
        {
            for (int j {0}; j < m_size; ++j)
            {
                m_tiles[i][j] = Tile {++count % 16};
            }
        }
    }

    constexpr int board_size() const
    {
        return m_size;
    }

    const Tile& operator[](size_t i, size_t j) const
    {
        return m_tiles[i][j];
    }

    Tile& operator[](size_t i, size_t j)
    {
        return m_tiles[i][j];
    }

    Point empty_tile_position() const
    {
        for (int i {0}; i < m_size; ++i)
        {
            for (int j {0}; j < m_size; ++j)
            {
                if (m_tiles[i][j].is_empty())
                {
                    return {j, i};
                }
            }
        }
        assert(false && "The empty tile was not found.");
        return {-1, -1};
    }

    void swap_tiles(const Point& lhs, const Point& rhs)
    {
        std::swap(m_tiles[lhs.y][lhs.x], m_tiles[rhs.y][rhs.x]);
    }

    bool move_tile(Direction direction)
    {
        const Point empty_tile_point {empty_tile_position()};
        const Point adjacent {empty_tile_point.get_adjacent_point(-direction)};
        if ((adjacent.x >= 0) && (adjacent.x < m_size) && (adjacent.y >= 0)
            && (adjacent.y < m_size))
        {
            swap_tiles(empty_tile_point, adjacent);
            return true;
        }
        else
        {
            return false;
        }
    }

    static Board random_board(int move_count)
    {
        Board board {};
        for (int i {0}; i < move_count;)
        {
            if (board.move_tile(Direction::random_direction()))
            {
                ++i;
            }
        }

        while (board.empty_tile_position().x != m_size - 1)
        {
            board.move_tile(Direction::left);
        }

        while (board.empty_tile_position().y != m_size - 1)
        {
            board.move_tile(Direction::up);
        }

        return board;
    }

    bool is_solved() const
    {
        int count {0};

        for (int i {0}; i < m_size; ++i)
        {
            for (int j {0}; j < m_size; ++j)
            {
                if (m_tiles[i][j].get_num() != ++count % 16)
                {
                    return false;
                }
            }
        }

        return true;
    }
};

std::ostream& operator<<(std::ostream& stream, const Board& board)
{
    // Before drawing always print empty lines so that only one board appears at
    // a time and is at the bottom of the window, because console window scrolls
    // automatically when there is no enough space.
    for (int i {0}; i < g_console_lines; ++i)
    {
        std::cout << "\n";
    }

    for (int i {0}; i < board.board_size(); ++i)
    {
        for (int j {0}; j < board.board_size(); ++j)
        {
            stream << board[i, j];
        }
        stream << "\n";
    }

    return stream;
}

/*
Supported commands:
    - 'w' -> slide up;
    - 'a' -> slide left;
    - 's' -> slide down;
    - 'd' -> slide right;
    - 'q' -> quit the game.
*/
namespace UserInput
{

static constexpr char valid_letters[] {'a', 'd', 's', 'w', 'q'};

Direction direction_from_input(char letter)
{
    switch (letter)
    {
        case 'a':
            return Direction::left;
        case 'd':
            return Direction::right;
        case 's':
            return Direction::down;
        case 'w':
            return Direction::up;
        default:
            assert(false && "Unsupported direction was passed.");
            return Direction::up;
    }
}

char get_letter()
{
    return Input::get_input<char>(std::span<const char>(valid_letters));
};

} // namespace UserInput

[[maybe_unused]]
static void game_loop()
{
    auto board {Board::random_board(5)};
    std::cout << board;

    int move_count {0};
    while (!board.is_solved())
    {
        const char letter {UserInput::get_letter()};
        if (letter == 'q')
        {
            std::cout << "\n\nBye! (" << move_count << " moves)\n\n";
            return;
        }

        if (board.move_tile(UserInput::direction_from_input(letter)))
        {
            std::cout << board;
            ++move_count;
        }
    }

    std::cout << "\n\nYou won! (" << move_count << " moves)\n\n";
}

int main()
{
    game_loop();

    return 0;
}
