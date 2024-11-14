#include <iostream>

template <typename T>
class Pair1
{
  public:
    using value_type = T;

    constexpr Pair1(const value_type& first, const value_type& second)
        : m_first {first}, m_second {second}
    {
    }

    constexpr const value_type& first() const
    {
        return m_first;
    }

    constexpr const value_type& second() const
    {
        return m_second;
    }

  private:
    value_type m_first;
    value_type m_second;
};

template <typename T, typename U>
class Pair
{
  public:
    constexpr Pair(const T& first, const U& second)
        : m_first {first}, m_second {second}
    {
    }

    constexpr const T& first() const
    {
        return m_first;
    }

    constexpr const U& second() const
    {
        return m_second;
    }

  private:
    T m_first;
    U m_second;
};

template <typename T>
class StringValuePair : public Pair<std::string, T>
{
    public:
      StringValuePair(std::string_view s, const T& value)
          : Pair<std::string, T> {std::string {s}, value}
      {
      }
};

static void run_1()
{
    Pair1<int> p1 {5, 8};
    std::cout << "Pair: " << p1.first() << ' ' << p1.second() << '\n';

    const Pair1<double> p2 {2.3, 4.5};
    std::cout << "Pair: " << p2.first() << ' ' << p2.second() << '\n';
}

static void run_2()
{
    Pair<int, double> p1 {5, 6.7};
    std::cout << "Pair: " << p1.first() << ' ' << p1.second() << '\n';

    const Pair<double, int> p2 {2.3, 4};
    std::cout << "Pair: " << p2.first() << ' ' << p2.second() << '\n';
}

static void run_3()
{
    StringValuePair<int> svp {"Hello", 5};
    std::cout << "Pair: " << svp.first() << ' ' << svp.second() << '\n';
}

int main()
{
    run_1();
    run_2();
    run_3();

    return 0;
}
