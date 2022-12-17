#include <iostream>
#include <string>

class BaseComponent;

/**
 * @brief The Mediator interface declares a method used by components to notify
 * the mediator about various events. The Mediator may react to these events
 * and pass the execution to other components.
 */
class Mediator
{
public:
    virtual void Notify(BaseComponent* sender, std::string event) const = 0;
};

/**
 * @brief The Base Component provides the basic functionality of storing a
 * mediator's instance inside component objects.
 */
class BaseComponent
{
public:
    BaseComponent(Mediator* mediator = nullptr) : mediator_(mediator) {}

    void SetMediator(Mediator* mediator)
    {
        this->mediator_ = mediator;
    }

protected:
    Mediator* mediator_;
};

/**
 * @brief Concrete Components implement various functionality. They don't
 * depend on other components. They also don't depend on any concrete mediator
 * classes.
 */
class Component1 : public BaseComponent
{
public:
    void DoA()
    {
        std::cout << "Component1 does A.\n";
        this->mediator_->Notify(this, "A");
    }

    void DoB()
    {
        std::cout << "Component1 does B.\n";
        this->mediator_->Notify(this, "B");
    }
};

class Component2 : public BaseComponent
{
public:
    void DoC()
    {
        std::cout << "Component2 does C.\n";
        this->mediator_->Notify(this, "C");
    }

    void DoD()
    {
        std::cout << "Component2 does D.\n";
        this->mediator_->Notify(this, "D");
    }
};

/**
 * @brief Concrete Mediators implement cooperative behavior by coordinating
 * several components.
 */
class ConcreteMediator : public Mediator
{
public:
    ConcreteMediator(Component1* c1, Component2* c2) : component1_(c1), component2_(c2)
    {
        this->component1_->SetMediator(this);
        this->component2_->SetMediator(this);
    }

    void Notify(BaseComponent* sender, std::string event) const override
    {
        if (event == "A")
        {
            std::cout << "Mediator reacts on A and triggers following operations:\n";
            this->component2_->DoC();
        }

        if (event == "D")
        {
            std::cout << "Mediator reacts on D and triggers following operations:\n";
            this->component1_->DoB();
            this->component2_->DoC();
        }
    }

private:
    Component1* component1_;
    Component2* component2_;
};

void ClientCode()
{
    Component1* c1 = new Component1;
    Component2* c2 = new Component2;
    ConcreteMediator* mediator = new ConcreteMediator(c1, c2);

    std::cout << "Client triggers operation A.\n";
    c1->DoA();
    std::cout << "\nClient triggers operation D.\n";
    c2->DoD();

    delete c1;
    delete c2;
    delete mediator;
}

int main()
{
    ClientCode();
    return 0;
}

// Output:
// Client triggers operation A.
// Component1 does A.
// Mediator reacts on A and triggers following operations:
// Component2 does C.

// Client triggers operation D.
// Component2 does D.
// Mediator reacts on D and triggers following operations:
// Component1 does B.
// Component2 does C.