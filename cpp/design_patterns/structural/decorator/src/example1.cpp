#include <iostream>
#include <memory>
#include <vector>

/**
 * The base Component interface defines operations that can be altered by
 * decorators.
 */
class Component
{
  public:
    virtual ~Component()
    {
        std::cout << "Destructor: Interface component\n";
    }

    virtual std::string operation() const = 0;
};

/**
 * Concrete Components provide default implementations of the operations. There
 * might be several variations of these classes.
 */
class ConcreteComponent : public Component
{
  public:
    ~ConcreteComponent()
    {
        std::cout << "Destructor: ConcreteComponent\n";
    }

    std::string operation() const override
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
    ~Decorator()
    {
        std::cout << "Destructor: Decorator\n";
    }

    Decorator(Component* component) : m_component_ptr {component} {}

    Decorator(const Decorator&) = default;
    Decorator& operator=(const Decorator&) = default;

    /**
     * The Decorator delegates all work to the wrapped component.
     */
    std::string operation() const override
    {
        return m_component_ptr->operation();
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

    ~ConcreteDecoratorA()
    {
        std::cout << "Destructor: ConcreteDecoratorA\n";
    }

    std::string operation() const override
    {
        return "ConcreteDecoratorA(" + Decorator::operation() + ")";
    }
};

/**
 * Decorators can execute their behavior either before or after the call to a
 * wrapped object.
 */
class ConcreteDecoratorB : public Decorator
{
  public:
    ConcreteDecoratorB(Component* component_ptr) : Decorator {component_ptr} {}

    ~ConcreteDecoratorB()
    {
        std::cout << "Destructor: ConcreteDecoratorB\n";
    }

    std::string operation() const override
    {
        return "ConcreteDecoratorB(" + Decorator::operation() + ")";
    }
};

/**
 * The client code works with all objects using the Component interface. This
 * way it can stay independent of the concrete classes of components it works
 * with.
 */
void ClientCode(Component* component_ptr)
{
    std::cout << "RESULT: " << component_ptr->operation() << '\n';
}

int main()
{
    std::vector<std::unique_ptr<Component>> vec {};

    auto d1 {std::make_unique<ConcreteComponent>()};
    auto d2 {std::make_unique<ConcreteDecoratorA>(d1.get())};
    auto d3 {std::make_unique<ConcreteDecoratorB>(d2.get())};

    vec.push_back(std::move(d1));
    vec.push_back(std::move(d2));
    vec.push_back(std::move(d3));

    for (auto& p : vec)
    {
        ClientCode(p.get());
    }
    std::cout << '\n';

    return 0;
}
