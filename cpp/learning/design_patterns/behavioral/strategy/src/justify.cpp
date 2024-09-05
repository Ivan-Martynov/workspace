#include <cstring>
#include <fstream>
#include <iostream>

class Strategy;

class TestBed
{
  private:
    Strategy* m_strategy_ptr;

  public:
    enum StrategyType
    {
        Dummy,
        Left,
        Right,
        Center,
    };

    TestBed() : m_strategy_ptr {nullptr} {}

    void set_strategy(int type, int width);

    void do_it();
};

class Strategy
{
  private:
    virtual void justify(char* line) = 0;

  protected:
    int m_width;

  public:
    Strategy(int width) : m_width {width} {}

    virtual ~Strategy() = default;

    void format()
    {
        char line[80];
        char word[30];
        std::ifstream in_file("quote.txt", std::ios::in);
        line[0] = '\0';

        in_file >> word;
        std::strcat(line, word);

        while (in_file >> word)
        {
            if (static_cast<int>(std::strlen(line) + std::strlen(word) + 1)
                > m_width)
            {
                justify(line);
            }
            else
            {
                std::strcat(line, " ");
            }

            std::strcat(line, word);
        }

        justify(line);
    }
};

class LeftStrategy : public Strategy
{
  private:
    void justify(char* line) override
    {
        std::cout << line << "\n";
        line[0] = '\0';
    }

  public:
    LeftStrategy(int width) : Strategy(width) {}
};

class RightStrategy : public Strategy
{
  private:
    void justify(char* line) override
    {
        char buf[80];
        int offset = m_width - std::strlen(line);
        std::memset(buf, ' ', 80);
        std::strcpy(&(buf[offset]), line);
        std::cout << buf << "\n";
        line[0] = '\0';
    }

  public:
    RightStrategy(int width) : Strategy(width) {}
};

class CenterStrategy : public Strategy
{
  private:
    void justify(char* line) override
    {
        char buf[80];
        int offset = (m_width - std::strlen(line)) / 2;
        std::memset(buf, ' ', 80);
        std::strcpy(&(buf[offset]), line);
        std::cout << buf << "\n";
        line[0] = '\0';
    }

  public:
    CenterStrategy(int width) : Strategy(width) {}
};

void TestBed::set_strategy(int type, int width)
{
    delete m_strategy_ptr;

    if (type == Left)
    {
        m_strategy_ptr = new LeftStrategy(width);
    }
    else if (type == Right)
    {
        m_strategy_ptr = new RightStrategy(width);
    }
    else if (type == Center)
    {
        m_strategy_ptr = new CenterStrategy(width);
    }
}

void TestBed::do_it()
{
    m_strategy_ptr->format();
}

int main()
{
    TestBed test {};

    int answer, width;
    std::cout << "Exit(0), Left(1), Right(2), Center(3): ";
    std::cin >> answer;

    while (answer)
    {
        std::cout << "Width: ";
        std::cin >> width;
        test.set_strategy(answer, width);
        test.do_it();
        std::cout << "Exit(0), Left(1), Right(2), Center(3): ";
        std::cin >> answer;
    }

    return 0;
}
