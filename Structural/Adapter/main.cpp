#include <iostream>
#include <string>

// Design Pattern - Adapter

/**
 * @brief Target defines the domain-specific interface used by the client code.
 */
class Target
{
public:

    virtual ~Target() = default;

    virtual std::string Request() const
    {
        return "Target: The default target's behavior.";
    }
};

/**
 * @brief The Adaptee contains some useful behavior, but its interface is
 * incompatible with the existing client code. The Adaptee needs some
 * adaptation befor ethe client code can use it.
 */
class Adaptee
{
public:
    std::string SpecificRequest() const
    {
        return ".eetpadA eht fo roivaheb laicepS";
    }
};

/**
 * @brief The Adapter makes the Adaptee's interface compatible with the
 * Target's interface.
 */
class Adapter : public Target
{
public:
    Adapter(Adaptee* adaptee) : adaptee_(adaptee) {}

    std::string Request() const override
    {
        std::string to_reverse = this->adaptee_->SpecificRequest();
        std::reverse(to_reverse.begin(), to_reverse.end());
        return "Adapter: (TRANSLATED) " + to_reverse;
    }

private:
    Adaptee* adaptee_;
};

/**
 * @brief The client code supports all classes that follow the Target
 * interface.
 */
void ClientCode(const Target* target)
{
    std::cout << target->Request();
}

int main()
{
    std::cout << "Client: I can work just fine with the Target objects:\n";
    Target* target = new Target();
    ClientCode(target);
    std::cout << "\n\n";

    Adaptee* adaptee = new Adaptee();
    std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
    std::cout << "Adaptee: " << adaptee->SpecificRequest();
    std::cout << "\n\n";

    std::cout << "Client: But I can work with it via the Adapter:\n";
    Adapter* adapter = new Adapter(adaptee);
    ClientCode(adapter);
    std::cout << "\n";

    delete target;
    delete adaptee;
    delete adapter;

    return 0;
}

// Output:
// Client: I can work just fine with the Target objects:
// Target: The default target's behavior.

// Client: The Adaptee class has a weird interface. See, I don't understand it:
// Adaptee: .eetpadA eht fo roivaheb laicepS

// Client: But I can work with it via the Adapter:
// Adapter: (TRANSLATED) Special behavior of the Adaptee.