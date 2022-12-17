#include <iostream>
#include <string>
#include <vector>
#include <ctime>

/**
 * @brief The Memento interface provides a way to retrieve the memento's
 * metadata, such as creation date or name. However, it doesn't expose the
 * Originator's state.
 */
class Memento
{
public:
    virtual ~Memento() {}
    virtual std::string GetName() const = 0;
    virtual std::string GetDate() const = 0;
    virtual std::string GetState() const = 0;
};

/**
 * @brief The ConcreteMemento contains the infrastructure for storing the
 * Originator's state.
 */
class ConcreteMemento : public Memento
{
public:
    ConcreteMemento(std::string state) : state_(state)
    {
        std::time_t now = std::time(0);
        this->date_ = std::ctime(&now);
    }

    /** NOTE: The Originator uses this method when restoring its state.*/
    std::string GetState() const override
    {
        return this->state_;
    }

    /** NOTE: The rest of the methods are used by the Caretaker to display
     * metadata.
     */
    std::string GetName() const override
    {
        return this->date_ + " / ( " + this->state_.substr(0, 9) + "...)";
    }

    std::string GetDate() const override
    {
        return this->date_;
    }

private:
    std::string state_;
    std::string date_;
};

/**
 * @brief The Originator holds some important state that may change over time.
 * It also defines a method for saving the state inside a memento and another
 * method for restoring the state from it.
 */
class Originator
{
public:
    Originator(std::string state) : state_(state)
    {
        std::cout << "Originator: My initial state is: " << this->state_ << "\n";
    }

    /**
     * NOTE: The Originator's business logic may affect its internal state.
     * Therefore, the client should backup the state before launching methods
     * of the business logic via the Save() method.
     */
    void DoSomething()
    {
        std::cout << "Originator: I'm doing something important.\n";
        this->state_ = this->GenerateRandomString(30);
        std::cout << "Originator: and my state has changed to: " << this->state_ << "\n";
    }

    /**
     * @brief Saves the current state inside a memento.
     */
    Memento* Save()
    {
        return new ConcreteMemento(this->state_);
    }

    /**
     * @brief Restores the Originator's state from a memento object.
     */
    void Restore(Memento* memento)
    {
        this->state_ = memento->GetState();
        std::cout << "Originator: My state has changed to: " << this->state_ << "\n";
    }

private:
    /**
     * NOTE: For the sake of simplicity, the originator's state is stored
     * inside a single variable.
     */
    std::string state_;

    std::string GenerateRandomString(int length = 10)
    {
        const char alphaNum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        int stringLength = sizeof(alphaNum) - 1;
        std::string randomString;
        for (size_t i = 0; i < length; i++)
        {
            randomString += alphaNum[std::rand() % stringLength];
        }

        return randomString;
    }
};

/**
 * @brief The Caretaker doesn't depend on the ConcreteMemento class. Therefore,
 * it doesn't have access to the originator's state, stored inside the memento.
 * It works with all mementos via the base Memento interface.
 */
class Caretaker
{
public:
    Caretaker(Originator* originator) : originator_(originator) {}

    ~Caretaker()
    {
        for (auto m : mementos_) delete m;
    }
    
    void Backup()
    {
        std::cout << "\nCaretaker: Saving Originator's state...\n";
        this->mementos_.push_back(this->originator_->Save());
    }

    void Undo()
    {
        if (!this->mementos_.size()) return;

        Memento* memento = this->mementos_.back();
        this->mementos_.pop_back();

        std::cout << "Caretaker: Restoring state to: " << memento->GetName() << "\n";
        try
        {
            this->originator_->Restore(memento);
        }
        catch (...)
        {
            this->Undo();
        }
    }

    void ShowHistory() const
    {
        std::cout << "Caretaker: Here's the list of mementos:\n";
        for (Memento* memento : this->mementos_)
        {
            std::cout << memento->GetName() << "\n";
        }
    }

private:
    std::vector<Memento*> mementos_;
    Originator* originator_;
};

void ClientCode()
{
    Originator* originator = new Originator("Super-duper-super-puper-super");
    Caretaker* caretaker = new Caretaker(originator);

    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    
    std::cout << "\n";
    caretaker->ShowHistory();

    std::cout << "\nClient: Now, let's rollback\n\n";
    caretaker->Undo();
    caretaker->Undo();
    caretaker->Undo();

    delete originator;
    delete caretaker;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    ClientCode();
    return 0;
}

// Output:
// Originator: My initial state is: Super-duper-super-puper-super

// Caretaker: Saving Originator's state...
// Originator: I'm doing something important.
// Originator: and my state has changed to: 0G0uNhetKlHmPKtL52MT2wsVApAQlP

// Caretaker: Saving Originator's state...
// Originator: I'm doing something important.
// Originator: and my state has changed to: BI4mhltZUuqWvT41jAtw6MhZtghySu

// Caretaker: Saving Originator's state...
// Originator: I'm doing something important.
// Originator: and my state has changed to: ze5IjxRKcpZNE7AAuOSRuQELWYLYao

// Caretaker: Here's the list of mementos:
// Tue Nov 29 22:58:18 2022
//  / ( Super-dup...)
// Tue Nov 29 22:58:18 2022
//  / ( 0G0uNhetK...)
// Tue Nov 29 22:58:18 2022
//  / ( BI4mhltZU...)

// Client: Now, let's rollback

// Caretaker: Restoring state to: Tue Nov 29 22:58:18 2022
//  / ( BI4mhltZU...)
// Originator: My state has changed to: BI4mhltZUuqWvT41jAtw6MhZtghySu
// Caretaker: Restoring state to: Tue Nov 29 22:58:18 2022
//  / ( 0G0uNhetK...)
// Originator: My state has changed to: 0G0uNhetKlHmPKtL52MT2wsVApAQlP
// Caretaker: Restoring state to: Tue Nov 29 22:58:18 2022
//  / ( Super-dup...)
// Originator: My state has changed to: Super-duper-super-puper-super