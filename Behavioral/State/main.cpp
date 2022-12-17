#include <iostream>
#include <typeinfo>

class Context;

/**
 * @brief The base State class declares methods that all Concrete State should
 * implement and also provides a backreference to the Context object,
 * associated with the State. This backreference can be used by States to
 * transition the Context to another state.
 */
class State
{
public:
    virtual ~State() {}

    void SetContext(Context* context)
    {
        this->context_ = context;
    }

    virtual void Handle1() = 0;
    virtual void Handle2() = 0;

protected:
    Context* context_;
};

/**
 * @brief The Context defines the interface of interest to clients. It also
 * maintains a reference to an instance of a State subclass, which represents
 * the current state of the Context.
 */
class Context
{
public:
    Context(State* state) : state_(nullptr)
    {
        this->TransitionTo(state);
    }

    void TransitionTo(State* state)
    {
        std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
        if (this->state_ != nullptr)
            delete this->state_;

        this->state_ = state;
        this->state_->SetContext(this);
    }

    /** NOTE: The Context delegates part of its behavior to the current State
     * object.
     */
    void Request1()
    {
        this->state_->Handle1();
    }

    void Request2()
    {
        this->state_->Handle2();
    }

private:
    State* state_;
};

/**
 * @brief Concrete States implement various behaviors, associated with a state
 * of the Context.
 */
class ConcreteStateA : public State
{
public:
    void Handle1() override;
    void Handle2() override
    {
        std::cout << "ConcreteStateA handles Request2.\n";
    }
};

class ConcreteStateB : public State
{
public:
    void Handle1() override
    {
        std::cout << "ConcreteStateB handles Request1.\n";
    }

    void Handle2() override
    {
        std::cout << "ConcreteStateB handles Request2.\n";
        std::cout << "ConcreteStateB wants to change the state of the context.\n";
        this->context_->TransitionTo(new ConcreteStateA);
    }
};

void ConcreteStateA::Handle1()
{
    std::cout << "ConcreteStateA handles Request1.\n";
    std::cout << "ConcreteStateA wants to change the state of the context.\n";
    this->context_->TransitionTo(new ConcreteStateB);
}


void ClientCode()
{
    Context* context = new Context(new ConcreteStateA);
    context->Request1();
    context->Request2();

    delete context;
}
int main()
{
    ClientCode();
    return 0;
}

// Output:
// Context: Transition to 14ConcreteStateA.
// ConcreteStateA handles Request1.
// ConcreteStateA wants to change the state of the context.
// Context: Transition to 14ConcreteStateB.
// ConcreteStateB handles Request2.
// ConcreteStateB wants to change the state of the context.
// Context: Transition to 14ConcreteStateA.