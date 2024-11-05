#include <iostream>
#include <cassert>
#include <cstdint>
#include <cmath>

class FixedPoint2
{
  private:
    std::int16_t m_base {};
    std::int8_t m_fraction {};

    constexpr void m_fix_sign()
    {
        if (m_fraction < 0)
        {
            if (m_base > 0)
            {
                m_base = -m_base;
            }
        }
        else if (m_base < 0)
        {
            m_fraction = -m_fraction;
        }
    }

    constexpr void m_fix_fraction()
    {
        while (std::abs(m_fraction) >= 100)
        {
            m_base += m_fraction / 100;
            m_fraction = m_fraction % 100;
        }
    }

  public:
    explicit constexpr FixedPoint2(std::int16_t base, std::int8_t fraction)
        : m_base {base}, m_fraction {fraction}
    {
        m_fix_sign();
        m_fix_fraction();
    }

    explicit constexpr FixedPoint2(double value)
        : FixedPoint2(static_cast<std::int16_t>(value),
              static_cast<std::int8_t>(
                  std::round((value - static_cast<std::int16_t>(value)) * 100)))
    {
    }

    constexpr bool operator==(const FixedPoint2& other) const
    {
        return (m_base == other.m_base) && (m_fraction == other.m_fraction);
    }

    constexpr FixedPoint2 operator-() const
    {
        return FixedPoint2 {static_cast<std::int16_t>(-m_base),
            static_cast<std::int8_t>(-m_fraction)};
    }

    constexpr operator double() const
    {
        return m_base + m_fraction * 0.01;
    }

    friend bool testDecimal(const FixedPoint2& fp);
    friend std::istream& operator>>(std::istream& stream, FixedPoint2& fp)
    {
        double value {};
        stream >> value;
        fp = FixedPoint2 {value};
        return stream;
    }
};

FixedPoint2 operator+(const FixedPoint2& lhs,const FixedPoint2& rhs)
{
    return FixedPoint2 {static_cast<double>(lhs) + static_cast<double>(rhs)};
}

bool testDecimal(const FixedPoint2& fp)
{
    if (fp.m_base >= 0)
    {
        return (fp.m_fraction >= 0) && (fp.m_fraction < 100);
    }
    else
    {
        return (fp.m_fraction <= 0) && (fp.m_fraction > -100);
    }
}

static void run_1()
{
    FixedPoint2 a {34, 56};
    std::cout << a << '\n';
    std::cout << static_cast<double>(a) << '\n';
    assert(static_cast<double>(a) == 34.56);

    FixedPoint2 b {-2, 8};
    assert(static_cast<double>(b) == -2.08);

    FixedPoint2 c {2, -8};
    assert(static_cast<double>(c) == -2.08);

    FixedPoint2 d {-2, -8};
    assert(static_cast<double>(d) == -2.08);

    FixedPoint2 e {0, -5};
    assert(static_cast<double>(e) == -0.05);

    FixedPoint2 f {0, 10};
    assert(static_cast<double>(f) == 0.1);
}

static void run_2()
{
    FixedPoint2 a {1, 104};
    std::cout << a << '\n';
    std::cout << static_cast<double>(a) << '\n';
    assert(static_cast<double>(a) == 2.04);
    assert(testDecimal(a));

    FixedPoint2 b {1, -104};
    assert(static_cast<double>(b) == -2.04);
    assert(testDecimal(b));

    FixedPoint2 c {-1, 104};
    assert(static_cast<double>(c) == -2.04);
    assert(testDecimal(c));

    FixedPoint2 d {-1, -104};
    assert(static_cast<double>(d) == -2.04);
    assert(testDecimal(d));
}

static void run_3()
{
    FixedPoint2 a {0.01};
    std::cout << a << "\n";
    assert(static_cast<double>(a) == 0.01);

    FixedPoint2 b {-0.01};
    std::cout << b << "\n";
    assert(static_cast<double>(b) == -0.01);

    FixedPoint2 c {1.9}; // make sure we handle single digit decimal
    std::cout << c << "\n";
    assert(static_cast<double>(c) == 1.9);

    FixedPoint2 d {5.01}; // stored as 5.0099999... so we'll need to round this
    assert(static_cast<double>(d) == 5.01);

    FixedPoint2 e {
        -5.01}; // stored as -5.0099999... so we'll need to round this
    assert(static_cast<double>(e) == -5.01);

    // Handle case where the argument's decimal rounds to 100 (need to increase
    // base by 1)
    FixedPoint2 f {106.9978}; // should be stored with base 107 and decimal 0
    assert(static_cast<double>(f) == 107.0);

    // Handle case where the argument's decimal rounds to -100 (need to decrease
    // base by 1)
    FixedPoint2 g {-106.9978}; // should be stored with base -107 and decimal 0
    assert(static_cast<double>(g) == -107.0);
}

static void run_4()
{
    assert(FixedPoint2 {0.75} == FixedPoint2 {0.75});    // Test equality true
    assert(!(FixedPoint2 {0.75} == FixedPoint2 {0.76})); // Test equality false

    // Test additional cases -- h/t to reader Sharjeel Safdar for these test
    // cases
    assert(FixedPoint2 {0.75} + FixedPoint2 {1.23}
           == FixedPoint2 {1.98}); // both positive, no decimal overflow
    assert(FixedPoint2 {0.75} + FixedPoint2 {1.50}
           == FixedPoint2 {2.25}); // both positive, with decimal overflow
    assert(FixedPoint2 {-0.75} + FixedPoint2 {-1.23}
           == FixedPoint2 {-1.98}); // both negative, no decimal overflow
    assert(FixedPoint2 {-0.75} + FixedPoint2 {-1.50}
           == FixedPoint2 {-2.25}); // both negative, with decimal overflow
    assert(FixedPoint2 {0.75} + FixedPoint2 {-1.23}
           == FixedPoint2 {-0.48}); // second negative, no decimal overflow
    assert(
        FixedPoint2 {0.75} + FixedPoint2 {-1.50}
        == FixedPoint2 {-0.75}); // second negative, possible decimal overflow
    assert(FixedPoint2 {-0.75} + FixedPoint2 {1.23}
           == FixedPoint2 {0.48}); // first negative, no decimal overflow
    assert(FixedPoint2 {-0.75} + FixedPoint2 {1.50}
           == FixedPoint2 {0.75}); // first negative, possible decimal overflow

    FixedPoint2 a {-0.48};
    assert(static_cast<double>(a) == -0.48);
    assert(static_cast<double>(-a) == 0.48);

    std::cout << "Enter a number: "; // enter 5.678
    std::cin >> a;
    std::cout << "You entered: " << a << '\n';
    assert(static_cast<double>(a) == 5.68);
}

int main()
{
    run_1();
    run_2();
    run_3();
    run_4();

    return 0;
}
