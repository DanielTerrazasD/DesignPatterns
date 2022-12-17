#include <iostream>
#include <string>
#include <list>

/**
 * @brief IObserver declares the basic operations of an Observer
 */
class IObserver
{
public:
    virtual ~IObserver() {}
    virtual void Update(const std::string& message_from_subject) = 0;
};

/**
 * @brief ISubject declares the basic operations of a Subject
 */
class ISubject
{
public:
    virtual ~ISubject() {}
    virtual void Attach(IObserver* observer) = 0;
    virtual void Detach(IObserver* observer) = 0;
    virtual void Notify() = 0;
};

/**
 * @brief Subject owns some important state and notifies observers when the
 * state changes.
 */
class Subject : public ISubject
{
public:
    void Attach(IObserver* observer) override
    {
        list_observer_.push_back(observer);
    }

    void Detach(IObserver* observer) override
    {
        list_observer_.remove(observer);
    }

    void Notify() override
    {
        std::list<IObserver*>::iterator it = list_observer_.begin();
        HowManyObservers();
        while (it != list_observer_.end())
        {
            (*it)->Update(message_);
            ++it;
        }
    }

    void CreateMessage(std::string message = "Empty")
    {
        this->message_ = message;
        Notify();
    }

    void HowManyObservers()
    {
        std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
    }

    /**
     * NOTE: Usually, the subscription logic is only a fraction of what a
     * Subject can really do. Subjects commonly hold some important business
     * logic, that triggers a notification method whenever something important
     * is about to happer (or after it).
     */
    void SomeBusinessLogic()
    {
        this->message_ = "change message";
        Notify();
        std::cout << "I'm about to do something important.\n";
    }

private:
    std::list<IObserver*> list_observer_;
    std::string message_;
};

/**
 * @brief Observer contains a reference to the observing subject and reacts
 * accordingly when an update occurs in the subject object.
 */
class Observer : public IObserver
{
public:
    Observer(Subject& subject) : subject_(subject)
    {
        this->subject_.Attach(this);
        std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_ << "\"\n";
        this->number_ = Observer::static_number_;
    }

    virtual ~Observer()
    {
        std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\"\n";
        // Optionally:
        // subject_.Detach(this);
    }

    void Update(const std::string& message_from_subject) override
    {
        message_from_subject_ = message_from_subject;
        PrintInfo();
    }

    void RemoveMeFromTheList()
    {
        subject_.Detach(this);
        std::cout << "Observer \"" << this->number_ << "\" removed from the list.\n";
    }

    void PrintInfo()
    {
        std::cout << "Observer \"" << this->number_ << "\" a new message is available --> "
                  << this->message_from_subject_ << "\n";
    }

private:
    std::string message_from_subject_;
    Subject& subject_;
    static int static_number_;
    int number_;
};

int Observer::static_number_ = 0;

void ClientCode()
{
    Subject* subject = new Subject;
    Observer* observer1 = new Observer(*subject);
    Observer* observer2 = new Observer(*subject);
    Observer* observer3 = new Observer(*subject);
    Observer* observer4;
    Observer* observer5;

    subject->CreateMessage("Hello World! :D");
    observer3->RemoveMeFromTheList();

    subject->CreateMessage("The weather is hot today! :P");
    observer4 = new Observer(*subject);

    observer2->RemoveMeFromTheList();
    observer5 = new Observer(*subject);

    subject->CreateMessage("My new car is great! ;)");
    observer5->RemoveMeFromTheList();
    
    observer4->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();

    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete subject;
}

int main()
{
    ClientCode();
    return 0;
}

// Output:
// Hi, I'm the Observer "1"
// Hi, I'm the Observer "2"
// Hi, I'm the Observer "3"
// There are 3 observers in the list.
// Observer "1" a new message is available --> Hello World! :D
// Observer "2" a new message is available --> Hello World! :D
// Observer "3" a new message is available --> Hello World! :D
// Observer "3" removed from the list.
// There are 2 observers in the list.
// Observer "1" a new message is available --> The weather is hot today! :P
// Observer "2" a new message is available --> The weather is hot today! :P
// Hi, I'm the Observer "4"
// Observer "2" removed from the list.
// Hi, I'm the Observer "5"
// There are 3 observers in the list.
// Observer "1" a new message is available --> My new car is great! ;)
// Observer "4" a new message is available --> My new car is great! ;)
// Observer "5" a new message is available --> My new car is great! ;)
// Observer "5" removed from the list.
// Observer "4" removed from the list.
// Observer "1" removed from the list.
// Goodbye, I was the Observer "5"
// Goodbye, I was the Observer "4"
// Goodbye, I was the Observer "3"
// Goodbye, I was the Observer "2"
// Goodbye, I was the Observer "1"