#include <iostream>
#include <string>

/**
 * @brief The Command interface declares a method for executing a command.
 */
class Command
{
public:
    virtual ~Command() {}
    virtual void Execute() const = 0;
};

/**
 * @brief Some commands can implement simple operations on their own.
 */
class SimpleCommand : public Command
{
public:
    explicit SimpleCommand(std::string pay_load) : pay_load_(pay_load) {}
    
    void Execute() const override
    {
        std::cout << "SimpleCommand: See, I can do simple things like printing ("
        << this->pay_load_ << ")\n";
    }

private:
    std::string pay_load_;
};

/**
 * @brief The Receiver classes contain some important business logic. They know
 * how to perform all kinds of operations, associated with carrying out a
 * request. In fact, any class may serve as a Receiver.
 */
class Receiver
{
public:
    void DoSomething(const std::string &a)
    {
        std::cout << "Receiver: Working on (" << a << ").\n";
    }

    void DoSomethingElse(const std::string &b)
    {
        std::cout << "Receiver: Also working on (" << b << ").\n";
    }
};

/**
 * @brief However, some commands can delegate more complex operations to other
 * objects, called "Receivers".
 */
class ComplexCommand : public Command
{
public:
    /**
     * NOTE: Complex commands can accept one or several receiver objects along
     * with any context data via the constructor.
     */
    ComplexCommand(Receiver* receiver, std::string a, std::string b) :
        receiver_(receiver), a_(a), b_(b) {}

    void Execute() const override
    {
        std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
        this->receiver_->DoSomething(this->a_);
        this->receiver_->DoSomethingElse(this->b_);
    }

private:
    Receiver* receiver_;
    std::string a_;
    std::string b_;
};

/**
 * @brief The Invoker is associated with one or several commands. It sends a
 * request to the command.
 */
class Invoker
{
public:
    ~Invoker()
    {
        delete on_start_;
        delete on_finish_;
    }

    void SetOnStart(Command* command)
    {
        this->on_start_ = command;
    }

    void SetOnFinish(Command* command)
    {
        this->on_finish_ = command;
    }

    void DoSomethingImportant()
    {
        std::cout << "Invoker: Does anybody want something done before I begin?\n";
        if (this->on_start_)
        {
            this->on_start_->Execute();
        }

        std::cout << "Invoker: ...doing something really important...\n";
        std::cout << "Invoker: Does anybody want something done after I finish?\n";
        if (this->on_finish_)
        {
            this->on_finish_->Execute();
        }
    }

private:
    Command* on_start_;
    Command* on_finish_;
};

int main()
{
    /**
     * NOTE: The client code can parameterize an invoker with any commands.
     */
    Invoker* invoker = new Invoker;
    invoker->SetOnStart(new SimpleCommand("Say Hi!"));
    Receiver* receiver = new Receiver;
    invoker->SetOnFinish(new ComplexCommand(receiver, "Send email", "Save report"));
    invoker->DoSomethingImportant();

    delete invoker;
    delete receiver;
    return 0;
}

// Output:
// Invoker: Does anybody want something done before I begin?
// SimpleCommand: See, I can do simple things like printing (Say Hi!)
// Invoker: ...doing something really important...
// Invoker: Does anybody want something done after I finish?
// ComplexCommand: Complex stuff should be done by a receiver object.
// Receiver: Working on (Send email).
// Receiver: Also working on (Save report).