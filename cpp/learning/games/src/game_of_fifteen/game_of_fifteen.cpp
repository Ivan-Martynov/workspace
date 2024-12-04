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
#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"

#include <iostream>
#include <array>
#include <algorithm>
#include <cassert>

#include <AL/al.h>
#include <AL/alc.h>

#if 0
void check_al_errors(const std::string& filename, const std::uint_fast32_t line)
{
    ALCenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error)
        {
            case AL_INVALID_NAME:
                std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to "
                             "an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                std::cerr << "AL_INVALID_ENUM: an invalid enum value was "
                             "passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to "
                             "an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                std::cerr << "AL_INVALID_OPERATION: the requested operation is "
                             "not valid";
                break;
            case AL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation "
                             "resulted in OpenAL running out of memory";
                break;
            default:
                std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
    }
}

void check_alc_errors(const std::string& filename,
    const std::uint_fast32_t line, ALCdevice* device)
{
    ALCenum error = alcGetError(device);
    if (error != ALC_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error)
        {
            case ALC_INVALID_VALUE:
                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed "
                             "to an OpenAL function";
                break;
            case ALC_INVALID_DEVICE:
                std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to "
                             "an OpenAL function";
                break;
            case ALC_INVALID_CONTEXT:
                std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to "
                             "an OpenAL function";
                break;
            case ALC_INVALID_ENUM:
                std::cerr << "ALC_INVALID_ENUM: an unknown enum value was "
                             "passed to an OpenAL function";
                break;
            case ALC_OUT_OF_MEMORY:
                std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was "
                             "passed to an OpenAL function";
                break;
            default:
                std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
    }
}

template <typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line,
    alFunction function, Params... params) ->
    typename std::enable_if<
        std::is_same<void, decltype(function(params...))>::value,
        decltype(function(params...))>::type
{
    function(std::forward<Params>(params)...);
    check_al_errors(filename, line);
}

template <typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line,
    alFunction function, Params... params) ->
    typename std::enable_if<
        !std::is_same<void, decltype(function(params...))>::value,
        decltype(function(params...))>::type
{
    auto ret = function(std::forward<Params>(params)...);
    check_al_errors(filename, line);
    return ret;
}

template <typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename, const std::uint_fast32_t line,
    alcFunction function, ALCdevice* device, Params... params) ->
    typename std::enable_if<
        std::is_same<void, decltype(function(params...))>::value,
        decltype(function(params...))>::type
{
    function(std::forward<Params>(params)...);
    check_alc_errors(filename, line, device);
}

template <typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename, const std::uint_fast32_t line,
    alcFunction function, ALCdevice* device, Params... params) ->
    typename std::enable_if<
        !std::is_same<void, decltype(function(params...))>::value,
        decltype(function(params...))>::type
{
    auto ret = function(std::forward<Params>(params)...);
    check_alc_errors(filename, line, device);
    return ret;
}

#define alCall(function, ...)                                                  \
    alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...)                                         \
    alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)
#endif

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

    constexpr Type get_type() const { return m_type; }

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

    bool operator!=(const Point& other) const { return !(*this == other); }

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

    constexpr int get_num() const { return m_index; }

    constexpr bool is_empty() const { return get_num() == 0; }

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

    constexpr int board_size() const { return m_size; }

    const Tile& operator[](size_t i, size_t j) const { return m_tiles[i][j]; }

    Tile& operator[](size_t i, size_t j) { return m_tiles[i][j]; }

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
    auto board {Board::random_board(1000)};
    std::cout << board;

    Marvin::SoundDevice::get();
    const auto background {Marvin::SoundBuffer::get()->add_sound_effect(
        "assets/audio/background.wav")};
    const auto hit_sound {Marvin::SoundBuffer::get()->add_sound_effect(
        "assets/audio/hit.wav")};

    Marvin::SoundSource background_source {};
    background_source.set_looping(true);
    background_source.set_gain(0.05f);
    background_source.play(background);

    Marvin::SoundSource hit_source {};
    hit_source.set_gain(0.25f);

    int move_count {0};
    while (!board.is_solved())
    {
        const char letter {UserInput::get_letter()};
        if (letter == 'q')
        {
            std::cout << "\n\nBye! (" << move_count << " moves)\n\n";
            return;
        }

        if (letter == 's'){
            background_source.pause();
        }
        else if (letter == 'd')
        {
            background_source.play();
        }

        if (board.move_tile(UserInput::direction_from_input(letter)))
        {
            hit_source.play(hit_sound);
            std::cout << board;
            ++move_count;
        }
    }

    std::cout << "\n\nYou won! (" << move_count << " moves)\n\n";
}

    [[maybe_unused]]
void audio_devices_names()
{
    //ALCdevice* device {};

    //std::vector<std::string> names {};
//    ALC_DEFAULT_DEVICE_SPECIFIER
    //const ALCchar* devices {alcGetString(nullptr, ALC_DEVICE_SPECIFIER)};
    //if (!alcCall(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER))
    //    return names;

#if 1
    std::cout << "ALC_DEVICE_SPECIFIER = "
              << alcGetString(nullptr, ALC_DEVICE_SPECIFIER)
              << "\n";
    std::cout << "ALC_DEFAULT_DEVICE_SPECIFIER = "
              << alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER)
              << "\n";
    std::cout << "ALC_DEFAULT_ALL_DEVICES_SPECIFIER = "
              << alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER)
              << "\n";
    std::cout << "ALC_CAPTURE_DEVICE_SPECIFIER = "
              << alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER)
              << "\n";
    std::cout << "ALC_ALL_DEVICES_SPECIFIER = "
              << alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER)
              << "\n";
    std::cout << "ALC_INVALID_DEVICE = "
              << alcGetString(nullptr, ALC_INVALID_DEVICE)
              << "\n";
    std::cout << "ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER = "
              << alcGetString(nullptr, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER)
              << "\n";
#endif

   // const char* ptr = devices;
   // do
   // {
   //     names.emplace_back(ptr);
   //     ptr += names.back().size() + 1;
   // } while (*(ptr + 1) != '\0');

   // return names;
}

[[maybe_unused]]
static void test_sound_device()
{
    [[maybe_unused]]
    auto* device_ptr {Marvin::SoundDevice::get()};
    auto* sound_buffer_ptr {Marvin::SoundBuffer::get()};
    [[maybe_unused]]
    const auto sound1 {
        sound_buffer_ptr->add_sound_effect("assets/audio/background.wav")};
    [[maybe_unused]]
    const auto sound2 {
        sound_buffer_ptr->add_sound_effect("assets/audio/hit.wav")};

    Marvin::SoundSource source {};
    source.set_looping(true);
    source.play(sound1);
}

int main()
{
    //audio_devices_names();

    //test_sound_device();

    game_loop();

    return 0;
}
