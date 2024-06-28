#include <iostream>
#include <queue>

class Giant
{
  private:
    int m_id;
    static int s_next;

  public:
    Giant(): m_id{ s_next++ } {}

    void fee() { std::cout << m_id << "-fee "; }

    void phi() { std::cout << m_id << "-phi "; }

    void pheaux() { std::cout << m_id << "-pheaux "; }
};

int Giant::s_next = 0;

class Command
{
  public:
    typedef void(Giant::* Action)();
    Command(Giant* object, Action method): m_object {object}, m_method {method}
    {}

    void execute()
    {
        (m_object->*m_method)();
    }

  private:
    Giant* m_object;
    Action m_method;
};

int main()
{
    Command* input[] {
        new Command(new Giant, &Giant::fee),
        new Command(new Giant, &Giant::phi),
        new Command(new Giant, &Giant::pheaux),
        new Command(new Giant, &Giant::fee),
        new Command(new Giant, &Giant::phi),
        new Command(new Giant, &Giant::pheaux),
    };

    std::queue<Command*> que {};
    for (int i = 0; i < 6; ++i)
    {
        que.push(input[i]);
    }
    for (; !que.empty(); que.pop())
    {
        que.front()->execute();
    }

    std::cout << "\n";

    return 0;
}
