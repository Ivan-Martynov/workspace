#include <cmath>
#include <iostream>
#include <string>

/*
 * The command interface declaring a method for executing a command.
 */
class Command
{
  public:
    virtual ~Command() {}

    virtual void execute() const = 0;
};

// Simple command can implement simple operations on their own.
class SimpleCommand : public Command
{
  private:
    std::string m_pay_load;

  public:
    explicit SimpleCommand(const std::string& pay_load) : m_pay_load(pay_load)
    {
    }

    void execute() const override
    {
        std::cout << "SimpleCommand: simple print: (" << m_pay_load << ")\n";
    }
};

/*
 * The receiver classes contain business logic. They know how to perform
 * operations, associated with carrying out a request.
 */
class Receiver
{
  public:
    void do_something(const std::string& s)
    {
        std::cout << "Receiver: working on (" << s << ".)\n";
    }

    void do_something_else(const std::string& s)
    {
        std::cout << "Receiver: also working on (" << s << ".)\n";
    }
};

// Some commands can delegat more complex operations to other objects, called
// receivers.
class ComplexCommand : public Command
{
  private:
    Receiver* m_receiver_ptr;

    // Context data.
    std::string m_a;
    std::string m_b;

  public:
    // Complex commands can accept one or several receiver objects along with
    // any context data via their constructor.
    ComplexCommand(
        Receiver* receiver_ptr, const std::string& a, const std::string& b)
        : m_receiver_ptr {receiver_ptr}, m_a {a}, m_b {b}
    {
    }

    ComplexCommand(const ComplexCommand&) = default;
    ComplexCommand& operator=(const ComplexCommand&) = default;

    void execute() const override
    {
        std::cout << "ComplexCommand: complex stuff done by a receiver\n";
        m_receiver_ptr->do_something(m_a);
        m_receiver_ptr->do_something_else(m_b);
    }
};

/*
 * The invoker is associated with one or several commands. It sends a request to
 * the command.
 */
class Invoker
{
  private:
    Command* m_on_start_ptr;
    Command* m_on_finish_ptr;

  public:
    ~Invoker()
    {
        delete m_on_start_ptr;
        delete m_on_finish_ptr;
    }

    void set_on_start(Command* command_ptr)
    {
        m_on_start_ptr = command_ptr;
    }

    void set_on_finish(Command* command_ptr)
    {
        m_on_finish_ptr = command_ptr;
    }

    /*
     * The invoker doesn't depend on concrete command or receiver classes. The
     * invoker passes a request to a receiver indirectly, by executing a
     * command.
     */
    void do_something()
    {
        std::cout << "Invoker: anything before I begin?\n";
        if (m_on_start_ptr)
        {
            m_on_start_ptr->execute();
        }
        std::cout << "Invoker: busy...\n";
        std::cout << "Invoker: anything before I quit?\n";
        if (m_on_finish_ptr)
        {
            m_on_finish_ptr->execute();
        }
    }
};

/*
 * The client code can parameterize an invoker with any command.
 */
int main()
{
    Invoker* invoker_ptr {new Invoker {}};
    invoker_ptr->set_on_start(new SimpleCommand("Say Hello!"));
    Receiver* receiver_ptr {new Receiver {}};
    invoker_ptr->set_on_finish(
        new ComplexCommand(receiver_ptr, "Send email", "Save report"));
    invoker_ptr->do_something();

    delete invoker_ptr;
    delete receiver_ptr;

    return 0;
}
