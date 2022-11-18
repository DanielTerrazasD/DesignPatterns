#include <iostream>
#include <string>

// Design Pattern - Bridge

/**
 * @brief Implementation defines the interface for all implementation classes.
 * It doesn't have to match the Abstraction's interface. In fact, the two
 * interfaces can be entirely different. Typically the Implementation interface
 * provides only primitive operations, while the Abstraction defines higher-
 * level operations based on those primitives.
 */
class Implementation
{
public:
    virtual ~Implementation(){}
    virtual std::string OperationImplementation() const = 0;
};

/**
 * @brief Each ConcreteImplementation corresponds to a specific platform and
 * implements the Implementation interface using that platform's API.
 */
class ConcreteImplementationA : public Implementation
{
public:
    std::string OperationImplementation() const override
    {
        return "ConcreteImplementationA: Here's the result of the platform A.\n";
    }
};

/**
 * @brief Each ConcreteImplementation corresponds to a specific platform and
 * implements the Implementation interface using that platform's API.
 */
class ConcreteImplementationB : public Implementation
{
public:
    std::string OperationImplementation() const override
    {
        return "ConcreteImplementationB: Here's the result of the platform B.\n";
    }
};

/**
 * @brief The Abstraction defines the interface for the control part of the two
 * class hierarchies. It maintains a reference to an object of the
 * Implementation hierarchy and delegates all the real work to this object.
 */
class Abstraction
{
public:
    Abstraction(Implementation* implementation) : implementation_(implementation) {}

    virtual ~Abstraction() {}

    virtual std::string Operation() const
    {
        return "Abstraction: Base operation with:\n" +
            this->implementation_->OperationImplementation();
    }

protected:
    Implementation* implementation_;
};

/**
 * @brief Abstraction class can be extended without changing the Implementation
 * classes.
 */
class ExtendedAbstraction : public Abstraction
{
public:
    ExtendedAbstraction(Implementation* implementation) : Abstraction(implementation) {}

    std::string Operation() const override
    {
        return "ExtendedAbstraction: Extended operation with: \n" +
            this->implementation_->OperationImplementation();
    }
};

/**
 * @brief Except for the initialization, where an Abstraction object gets
 * linked with a specific Implementation object, the client code should only
 * depend on the Abstraction class. This way, the client code can support any
 * abstraction-implementation combination.
 */
void ClientCode(const Abstraction& abstraction)
{
    std::cout << abstraction.Operation();
}

int main()
{
    Implementation* implementation = new ConcreteImplementationA;
    Abstraction* abstraction = new Abstraction(implementation);

    ClientCode(*abstraction);
    std::cout << std::endl;

    delete implementation;
    delete abstraction;

    implementation = new ConcreteImplementationB;
    abstraction = new ExtendedAbstraction(implementation);
    ClientCode(*abstraction);

    delete implementation;
    delete abstraction;

    return 0;
}

// Output:
// Abstraction: Base operation with:
// ConcreteImplementationA: Here's the result of the platform A.

// ExtendedAbstraction: Extended operation with:
// ConcreteImplementationB: Here's the result of the platform B.
