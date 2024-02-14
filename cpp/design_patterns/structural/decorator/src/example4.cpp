#include <iostream>
#include <string>
using namespace std;

class Interface
{
  public:
    virtual ~Interface() {}
    virtual void write(string&) = 0;
    virtual void read(string&) = 0;
};

class Core : public Interface
{
  public:
    ~Core()
    {
        cout << "dtor-Core\n";
    }

    void write(string& b)
    {
        b += "MESSAGE|";
    }

    void read(string&)
    {
        cout << "Core: " << "MESSAGE" << '\n';
    }
};

class Decorator : public Interface
{
    Interface* inner_ptr;

  public:
    Decorator(Interface* c_ptr) : inner_ptr {c_ptr} {}

    Decorator(const Decorator&) = default;
    Decorator& operator=(const Decorator&) = default;

    ~Decorator()
    {
        delete inner_ptr;
    }

    void write(string& b)
    {
        inner_ptr->write(b);
    }

    void read(string& b)
    {
        inner_ptr->read(b);
    }
};

class Wrapper : public Decorator
{
  private:
    string forward;
    string backward;

  public:
    Wrapper(Interface* c_ptr, const string str)
        : Decorator {c_ptr}, forward {str}, backward {}
    {
        for (string::const_reverse_iterator it {str.crbegin()};
             it != str.crend(); ++it)
        {
            backward += *it;
        }
    }

    ~Wrapper()
    {
        cout << "dtor-" << forward << "  ";
    }

    void write(string& b)
    {
        b += forward + ']';
        Decorator::write(b);
        b += backward + ']';
    }

    void read(string& b)
    {
        cout << "Wrapper: " << forward << '\n';
        Decorator::read(b);
        cout << "Wrapper: " << backward << '\n';
    }
};

int main()
{
    Interface* object_ptr {
        new Wrapper(new Wrapper(new Wrapper(new Core(), "123"), "abc"), "987")};

    string buf{};
    object_ptr->write(buf);

    cout << "main: " << buf << endl;

    object_ptr->read(buf);

    delete object_ptr;
}
