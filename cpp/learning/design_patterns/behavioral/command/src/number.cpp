#include <iostream>
#include <vector>

class Number
{
  public:
    void dubble(int& value)
    {
        value *= 2;
    }
};

class Command
{
  public:
    virtual void execute(int&) = 0;

    virtual ~Command() = default;
};

class SimpleCommand: public Command
{
  private:
    typedef void(Number::* Action)(int&);
    Number* receiver;
    Action action;

  public:
    SimpleCommand(Number* rec, Action act): receiver {rec}, action {act} {}
    SimpleCommand(const SimpleCommand& other) = default;
    SimpleCommand& operator=(const SimpleCommand& other) = default;

    void execute(int& num)
    {
        (receiver->*action)(num);
    }
};

class MacroCommand: public Command
{
  private:
    std::vector<Command*> list;

  public:
    MacroCommand(): list {} {}

    void add(Command* cmd)
    {
        list.push_back(cmd);
    }

    void execute(int& num)
    {
        for (auto l : list)
        {
            l->execute(num);
        }
    }
};

int main()
{
    Number object {};
    Command* commands[3];
    SimpleCommand simple {&object, &Number::dubble};
    commands[0] = &simple;

    MacroCommand two {};
    two.add(commands[0]);
    two.add(commands[0]);
    commands[1] = &two;

    MacroCommand four {};
    four.add(&two);
    four.add(&two);
    commands[2] = &four;

    while (true)
    {
        std::cout << "Enter number selection (0=2x 1=4x, 2=16x): ";
        int num {};
        int index {};
        std::cin >> num >> index;
        commands[index]-> execute(num);
        std::cout << "   " << num << "\n";
    }

    return 0;
}
