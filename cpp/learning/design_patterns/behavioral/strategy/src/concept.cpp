#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

class Strategy
{
  public:
    virtual ~Strategy() = default;

    virtual std::string do_algorithm(std::string_view data) const = 0;
};

class Context
{
  private:
    std::unique_ptr<Strategy> m_strategy_ptr;

  public:
    explicit Context(std::unique_ptr<Strategy>&& strategy_ptr = {})
        : m_strategy_ptr {std::move(strategy_ptr)}
    {
    }

    void set_strategy(std::unique_ptr<Strategy>&& strategy_ptr)
    {
        m_strategy_ptr = std::move(strategy_ptr);
    }

    void do_logic() const
    {
        if (m_strategy_ptr)
        {
            std::cout << "Context: sorting data using strategy (not sure how "
                         "it'll do it)\n";

            const std::string result {m_strategy_ptr->do_algorithm("aebcd")};

            std::cout << result << "\n";
        }
        else
        {
            std::cout << "Context: strategy not set\n";
        }
    }
};

class ConcreteStrategyA: public Strategy
{
  public:

    std::string do_algorithm(std::string_view data) const override
    {
        std::string result {data};
        std::sort(result.begin(), result.end());

        return result;
    }
};

class ConcreteStrategyB: public Strategy
{
  public:

    std::string do_algorithm(std::string_view data) const override
    {
        std::string result {data};
        std::sort(result.begin(), result.end(), std::greater<>());

        return result;
    }
};

int main()
{
    Context context {std::make_unique<ConcreteStrategyA>()};
    std::cout << "Cliend: strategy set to normal soring\n";
    context.do_logic();

    std::cout << "Cliend: strategy set to reverse soring\n";
    context.set_strategy(std::make_unique<ConcreteStrategyB>());
    context.do_logic();

    return 0;
}
