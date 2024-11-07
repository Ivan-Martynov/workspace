#include <iostream>

class Fruit
{
  public:
    explicit constexpr Fruit(std::string_view name, std::string_view color)
        : m_name {name}, m_color {color}
    {
    }

    const std::string& name() const
    {
        return m_name;
    }

    const std::string& color() const
    {
        return m_color;
    }

  private:
    std::string m_name;
    std::string m_color;
};

std::ostream& operator<<(std::ostream& stream, const Fruit& fruit)
{
    stream << fruit.name() << ", " << fruit.color();
    return stream;
}

class Apple : public Fruit
{
  public:
    explicit constexpr Apple(
        std::string_view name, std::string_view color, double fiber)
        : Fruit {name, color}, m_fiber {fiber}
    {
    }

    explicit constexpr Apple(std::string_view color, double fiber = 0.5)
        : Fruit {"apple", color}, m_fiber {fiber}
    {
    }

    friend std::ostream& operator<<(std::ostream& stream, const Apple& apple)
    {
        stream << "Apple(" << static_cast<const Fruit&>(apple) << ", "
               << apple.m_fiber << ")";
        return stream;
    }

  private:
    double m_fiber;
};

class GrannySmith : public Apple
{
  public:
    GrannySmith() : Apple {"Granny smith apple", "green", 0.5} {}
};

class Banana : public Fruit
{
  public:
    explicit constexpr Banana(
        std::string_view name = "banana", std::string_view color = "yellow")
        : Fruit {name, color}
    {
    }
};

std::ostream& operator<<(std::ostream& stream, const Banana& banana)
{
    stream << "Banana(" << static_cast<const Fruit&>(banana) << ")";
    return stream;
}

int main()
{
    const Apple a {"Red delicious", "red", 4.2};
    std::cout << a << '\n';

    const Banana b {"Cavendish", "yellow"};
    std::cout << b << '\n';

    Apple a2{ "red" };
	Banana b2;
    GrannySmith c;

	std::cout << "My " << a2.name() << " is " << a2.color() << ".\n";
	std::cout << "My " << b2.name() << " is " << b2.color() << ".\n";
	std::cout << "My " << c.name() << " is " <<c.color() << ".\n";

    return 0;
}
