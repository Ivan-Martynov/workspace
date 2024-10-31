#include "Random.h"

#include <iostream>
#include <vector>

namespace Settings
{

constexpr int guess_count {6};

} // namespace Settings


namespace WordList
{

const std::vector<std::string_view> words {"mystery", "broccoli", "account",
    "almost", "spaghetti", "opinion", "beautiful", "distance", "luggage"};

std::string_view pick_random_word()
{
    return words[Random::get(0, words.size() - 1)];
}

} // namespace WordList

class Session
{
  private:
    std::string_view m_word;
    std::vector<bool> m_letters;
    int m_guess_count {Settings::guess_count};
    bool m_is_won {false};

    std::size_t to_index(char c) const
    {
        return static_cast<std::size_t>((c % 32) - 1);
    }

  public:
    explicit Session() : m_word {WordList::pick_random_word()}, m_letters(26) {}

    std::string_view get_word() const
    {
        return m_word;
    }

    bool is_letter_guessed(char letter) const
    {
        return m_letters[to_index(letter)];
    }

    void set_letter_guessed(char letter)
    {
        m_letters[to_index(letter)] = true;
    }

    int guess_count() const
    {
        return m_guess_count;
    }

    void remove_guess()
    {
        --m_guess_count;
    }

    bool has_letter(char letter) const
    {
        return m_word.find(letter) != std::string_view::npos;
    }

    bool game_won() const
    {
        for (const auto c : get_word())
        {
            if (!is_letter_guessed(c))
            {
                return false;
            }
        }
        return true;
    }

    void set_victory(bool b)
    {
        m_is_won = b;
    }
};

static void draw(const Session& session)
{
    std::cout << "\nThe word: ";
    for (const auto c : session.get_word())
    {
        if (session.is_letter_guessed(c))
        {
            std::cout << c;
        }
        else
        {
            std::cout << '_';
        }
    }

    std::cout << " Wrong guesses: ";
    for (int i {0}; i < session.guess_count(); ++i)
    {
        std::cout << '+';
    }

    for (char c {'a'}; c <= 'z'; ++c)
    {
        if (session.is_letter_guessed(c) && !session.has_letter(c))
        {
            std::cout << c;
        }
    }
    std::cout << "\n";
}

static char get_input(Session& session)
{
    constexpr std::string_view invalid_input_message {
        "That wasn't a valid input. Try again.\n"};

    while (true)
    {
        std::cout << "Enter your next letter: ";

        char letter {};
        std::cin >> letter;

        auto clear_input_lambda {[]() {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }};

        if (!std::cin)
        {
            std::cin.clear();
            std::cerr << invalid_input_message;
            clear_input_lambda();
            continue;
        }

        // Clear any extraneous input.
        clear_input_lambda();

        letter = std::tolower(letter);
        if (!std::isalpha(letter))
        {
            std::cerr << invalid_input_message;
            continue;
        }

        if (session.is_letter_guessed(letter))
        {
            std::cout << "You alreade guessed that. Try again.\n";
            continue;
        }

        return letter;
    }
}

static void handle_guess(Session& session, char letter)
{
    session.set_letter_guessed(letter);
    if (session.has_letter(letter))
    {
        std::cout << "Yes, '" << letter << "' is in the word!\n";
    }
    else
    {
        std::cout << "No, '" << letter << "' is not in the word!\n";
        session.remove_guess();
    }
}

int main()
{
    std::cout << "Welcome to C++man (a variant of Hangman)\n";
    std::cout << "To win: guess the word.  To lose: run out of pluses.\n";

    Session session {};

    while ((session.guess_count() > 0) && !session.game_won())
    {
        draw(session);
        handle_guess(session, get_input(session));
    }

    draw(session);

    if (session.guess_count() > 0)
    {
        std::cout << "You won!\n";
    }
    else
    {
        std::cout << "You lost! The word was " << session.get_word() << ".\n";
    }

    return 0;
}
