#include "Random.h"

#include <iostream>
#include <array>
#include <cassert>
#include <algorithm>

namespace Settings
{

constexpr int player_bust {21};
constexpr int dealer_stop {17};

} // namespace Settings

struct Card
{
    enum Rank
    {
        rank_ace,
        rank_2,
        rank_3,
        rank_4,
        rank_5,
        rank_6,
        rank_7,
        rank_8,
        rank_9,
        rank_10,
        rank_jack,
        rank_queen,
        rank_king,

        max_ranks,
    };

    enum Suit
    {
        suit_club,
        suit_diamond,
        suit_heart,
        suit_spade,

        max_suits,
    };

    static constexpr std::array<Rank, max_ranks> allRanks {
        rank_ace,
        rank_2,
        rank_3,
        rank_4,
        rank_5,
        rank_6,
        rank_7,
        rank_8,
        rank_9,
        rank_10,
        rank_jack,
        rank_queen,
        rank_king,
    };

    static constexpr std::array<Suit, max_suits> allSuits {
        suit_club,
        suit_diamond,
        suit_heart,
        suit_spade,
    };

    Rank rank;
    Suit suit;

    friend std::ostream& operator<<(std::ostream& stream, const Card& card)
    {
        static constexpr std::array<char, max_ranks> rank_names {
            'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
        static constexpr std::array<char, max_suits> suit_names {
            'C', 'D', 'H', 'S'};

        stream << rank_names[card.rank] << suit_names[card.suit];
        return stream;
    }

    int value() const
    {
        static constexpr std::array rank_values {
            11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

        return rank_values[rank];
    }
};

class Deck
{
  private:
    static constexpr size_t m_card_count {52};
    size_t m_next_card {0};
    std::array<Card, m_card_count> m_cards {};

  public:
    explicit Deck()
    {
        size_t i {0};
        for (auto suit : Card::allSuits)
        {
            for (auto rank : Card::allRanks)
            {
                m_cards[i++] = Card {rank, suit};
            }
        }
    }

    Card deal()
    {
        // If ran out of cards, shuffle the deck again.
        if (m_next_card == m_card_count)
        {
            shuffle();
        }
        //assert((m_next_card != m_card_count) && "Deck::deal ran out of cards.");
        return m_cards[m_next_card++];
    }

    void shuffle()
    {
        std::shuffle(m_cards.begin(), m_cards.end(), Random::mt);
        m_next_card = 0;
    }
};

struct Player
{
    int score {0};
    int ace_count {0};
};

static char get_input()
{
    constexpr std::string_view invalid_input_message {
        "That wasn't a valid input. Try again.\n"};

    while (true)
    {
        std::cout << "[h] to hit or [s] to stand: ";

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
        if ((letter != 'h') && (letter != 's'))
        {
            std::cerr << invalid_input_message;
            continue;
        }

        return letter;
    }
}

static Card deal_card(Player& player, Deck& deck)
{
    const auto card {deck.deal()};
    if (card.rank == Card::rank_ace)
    {
        ++player.ace_count;
    }

    player.score += card.value();

    if ((player.score > Settings::player_bust) && (player.ace_count > 0))
    {
        player.score -= 10;
        --player.ace_count;
    }
    return card;
}

static void game_loop(Deck& deck)
{
    Player dealer {deck.deal().value()};
    Player player {deck.deal().value() + deck.deal().value()};

    std::cout << "The dealer is showing: " << dealer.score
              << "\nYou have score: " << player.score << "\n";

    while (player.score < Settings::player_bust)
    {
        if (get_input() == 'h')
        {
            const auto card {deal_card(player, deck)};
            std::cout << "You were dealt a " << card << ". You now have "
                      << player.score << "\n";
        }
        else
        {
            break;
        }
    }

    if (player.score > Settings::player_bust)
    {
        std::cout << "You went bust!";
        player.score = 0;
    }

    while ((player.score != 0) && (dealer.score < Settings::dealer_stop))
    {
        const auto card {deal_card(dealer, deck)};
        std::cout << "The dealer flips a " << card << ". They now have "
                  << dealer.score << "\n";
    }

    if (dealer.score > Settings::player_bust)
    {
        std::cout << "The dealer went bust!";
        dealer.score = 0;
    }

    std::cout << "\nYou " << (player.score > dealer.score ? "win" : "lose")
              << "!\n";
}

int main()
{
    Deck deck {};
    deck.shuffle();

    game_loop(deck);

    return 0;
}
