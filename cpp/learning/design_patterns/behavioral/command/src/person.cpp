#include <iostream>
#include <string>

class Person;

class Command
{
  private:
    Person* m_object;
    void (Person::* m_method)();

  public:
    Command(Person* obj = nullptr, void(Person::* method)() = nullptr):
        m_object {obj}, m_method {method} {}

    void execute() { (m_object->*m_method)(); }
};

class Person
{
private:
    std::string name;
    Command cmd;
public:
    Person(std::string n, Command c): name {n}, cmd {c} {};

    void talk()
    {
        // "this" is the sender, cmd has the receiver.
        std::cout << name << " is talking.\n";

        // ask the "black box" to callback the receiver.
        cmd.execute();
    }

    void pass_on()
    {
        std::cout << name << " is passing on.\n";
        // When the sender is ready to callback to the receiver, it calls
        // execute().
        cmd.execute();
    }

    void gossip()
    {
        std::cout << name << " is gossiping.\n";
        cmd.execute();
    }

    void listen()
    {
        std::cout << name << " is listening.\n";
    }
};

int main()
{
    // Instantiate an object for each "callback".
    // Pass each object to its future "sender".
    Person wilma { "Wilma", Command {} };
    // Betty will "execute" Wilma to call listen.
    Person betty { "Betty", Command {&wilma, &Person::listen} };
    // Barney will "execute" Betty to call gossip.
    Person barney { "Barney", Command {&betty, &Person::gossip} };
    // Fred will "execute" Barney to call pass_on.
    Person fred { "Fred", Command {&barney, &Person::pass_on} };
    fred.talk();

    return 0;
}
