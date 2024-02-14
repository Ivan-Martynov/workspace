#include <iostream>

/**
 * The base Component interface defines operations that can be altered by
 * decorators.
 */
class Component
{
  public:
    virtual ~Component() {}
    virtual std::string Operation() const = 0;
};

/**
 * Concrete Components provide default implementations of the operations. There
 * might be several variations of these classes.
 */
class ConcreteComponent : public Component
{
  public:
    std::string Operation() const override
    {
        return "ConcreteComponent";
    }
};

/**
 * The base Decorator class follows the same interface as the other components.
 * The primary purpose of this class is to define the wrapping interface for all
 * concrete decorators. The default implementation of the wrapping code might
 * include a field for storing a wrapped component and the means to initialize
 * it.
 */
class Decorator : public Component
{
    /**
     * @var Component
     */
  protected:
    Component* m_component_ptr;

  public:
    Decorator(Component* component) : m_component_ptr(component) {}

    Decorator(const Decorator&) = default;
    Decorator& operator=(const Decorator&) = default;

    /**
     * The Decorator delegates all work to the wrapped component.
     */
    std::string Operation() const override
    {
        return m_component_ptr->Operation();
    }
};

/**
 * Concrete Decorators call the wrapped object and alter its result in some way.
 */
class ConcreteDecoratorA : public Decorator
{
    /**
     * Decorators may call parent implementation of the operation, instead of
     * calling the wrapped object directly. This approach simplifies extension
     * of decorator classes.
     */
  public:
    ConcreteDecoratorA(Component* component_ptr) : Decorator {component_ptr} {}

    std::string Operation() const override
    {
        return "ConcreteDecoratorA(" + Decorator::Operation() + ")";
    }
};

/**
 * Decorators can execute their behavior either before or after the call to a
 * wrapped object.
 */
class ConcreteDecoratorB : public Decorator
{
  public:
    ConcreteDecoratorB(Component* component_ptr) : Decorator{component_ptr} {}

    std::string Operation() const override
    {
        return "ConcreteDecoratorB(" + Decorator::Operation() + ")";
    }
};

/**
 * The client code works with all objects using the Component interface. This
 * way it can stay independent of the concrete classes of components it works
 * with.
 */
void ClientCode(Component* component_ptr)
{
    std::cout << "RESULT: " << component_ptr->Operation();
}

int main()
{
    /**
     * This way the client code can support both simple components...
     */
    Component* simple_ptr {new ConcreteComponent {}};

    std::cout << "Client: I've got a simple component:\n";
    ClientCode(simple_ptr);
    std::cout << "\n\n";

    /**
     * ...as well as decorated ones.
     *
     * Note how decorators can wrap not only simple components but the other
     * decorators as well.
     */
    Component* decorator1_ptr {new ConcreteDecoratorA {simple_ptr}};
    Component* decorator2_ptr {new ConcreteDecoratorB {decorator1_ptr}};

    std::cout << "Client: Now I've got a decorated component:\n";
    ClientCode(decorator2_ptr);
    std::cout << "\n";

    delete simple_ptr;
    delete decorator1_ptr;
    delete decorator2_ptr;

    return 0;
}
