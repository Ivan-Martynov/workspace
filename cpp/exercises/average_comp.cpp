#include <iostream>
#include <cstdint>

class Average
{
  private:
    std::int32_t m_sum {0};
    int m_count {0};

  public:
    Average& operator+=(std::int32_t value)
    {
        m_sum += value;
        ++m_count;

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Average& avg)
    {
        stream << (avg.m_count ? static_cast<double>(avg.m_sum) / avg.m_count
                               : 0);
        return stream;
    }
};

int main()
{
    Average avg {};
    std::cout << avg << '\n';

    avg += 4;
    std::cout << avg << '\n'; // 4 / 1 = 4

    avg += 8;
    std::cout << avg << '\n'; // (4 + 8) / 2 = 6

    avg += 24;
    std::cout << avg << '\n'; // (4 + 8 + 24) / 3 = 12

    avg += -10;
    std::cout << avg << '\n'; // (4 + 8 + 24 - 10) / 4 = 6.5

    (avg += 6) += 10;         // 2 calls chained together
    std::cout << avg << '\n'; // (4 + 8 + 24 - 10 + 6 + 10) / 6 = 7

    Average copy {avg};
    std::cout << copy << '\n';

    return 0;
}
