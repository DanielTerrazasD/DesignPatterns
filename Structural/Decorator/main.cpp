#include <iostream>
#include <string>

/**
 * @brief The Component interface defines operation that can be altered
 * by decorators.
 */
class Component
{
public:
    virtual ~Component() {}
    virtual std::string Operation() const = 0;
};

/**
 * @brief Concrete Components provide default implementations of the
 * operations. There might be several variations of these classes.
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
 * @brief The base Decorator class follows the same interface as the other
 * components. The primary purpose of this class is to define the wrapping
 * interface for all concrete decorators. The default implementation of the
 * wrapping code might include a field for storing a wrapped component and the
 * means to initialize it.
 */
class Decorator : public Component
{
public:
    Decorator(Component* component) : component_(component) {}

    /**
     * @brief The Decorator delegates all work to the wrapped component.
     */
    std::string Operation() const override
    {
        return this->component_->Operation();
    }

protected:
    Component* component_;
};

/**
 * @brief Concrete Decorators call the wrapped object and alter its result in
 * some way.
 */
class ConcreteDecoratorA : public Decorator
{
public:
    ConcreteDecoratorA(Component* component) : Decorator(component) {}

    std::string Operation() const override 
    {
        return "ConcreteDecoratorA( " + Decorator::Operation() + " )";
    }
};

/**
 * @brief Concrete Decorators can execute their behavior either before or after
 * the call to a wrapped object.
 */
class ConcreteDecoratorB : public Decorator
{
public:
    ConcreteDecoratorB(Component* component) : Decorator(component) {}

    std::string Operation() const override
    {
        return "ConcreteDecoratorB( " + Decorator::Operation() + " )";
    }
};

/**
 * @brief The client code works with all objects using the Component interface.
 * This way it can stay independent of the concrete classes of components it
 * works with.
 */
void ClientCode(Component* component)
{
    std::cout << "RESULT: " << component->Operation();
}

int main()
{
    Component* simple = new ConcreteComponent;
    std::cout << "Client: I've got a simple component:\n";
    ClientCode(simple);
    std::cout << "\n\n";

    /**
     * NOTE: Note how decorators can wrap not only simple components but the
     * other decorators as well.
     */
    Component* decorator1 = new ConcreteDecoratorA(simple);
    Component* decorator2 = new ConcreteDecoratorB(decorator1);
    std::cout << "Client: Now I've got a decorated component:\n";
    ClientCode(decorator2);
    std::cout << "\n";

    delete simple;
    delete decorator1;
    delete decorator2;
    return 0;
}

// Output:
// Client: I've got a simple component:
// RESULT : ConcreteComponent

// Client : Now I've got a decorated component:
// RESULT : ConcreteDecoratorB(ConcreteDecoratorA(ConcreteComponent))
