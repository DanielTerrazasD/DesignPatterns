
#include <iostream>
#include <string>
#include <unordered_map>

// Design Pattern - Flyweight

/**
 * @brief The SharedState represents the intrinsic state of a flyweight.
 * The Flyweight class will store a SharedState object that can be reused
 * by the application.
 */
struct SharedState
{
    std::string brand_;
    std::string model_;
    std::string color_;

    SharedState(    const std::string& brand,
                    const std::string& model,
                    const std::string& color) :
        brand_(brand), model_(model), color_(color) {}

    friend std::ostream& operator<<(std::ostream& os, const SharedState& ss)
    {
        return os << "[ " << ss.brand_ << ", " << ss.model_ << ", " << ss.color_ << " ]";
    }
};

/**
 * @brief The UniqueState represents the extrinsic state of a flyweight.
 * This state depends on the context and it's always provided by the Client.
 */
struct UniqueState
{
    std::string owner_;
    std::string plates_;

    UniqueState(const std::string& owner, const std::string& plates) :
        owner_(owner), plates_(plates) {}

    friend std::ostream& operator<<(std::ostream& os, const UniqueState& us)
    {
        return os << "[ " << us.owner_ << ", " << us.plates_ << " ]";
    }
};

/**
 * @brief The Flyweight stores a common portion of the state (also called
 * intrinsic state) that belongs to multiple real business entities. The
 * Flyweight accepts the rest of the state (extrinsic state, unique for each
 * entity) via its method parameters.
 */
class Flyweight
{
public:
    Flyweight(const SharedState* shared_state) :
        shared_state_(new SharedState(*shared_state)) {}

    Flyweight(const Flyweight& other) :
        shared_state_(new SharedState(*other.shared_state_)) {}

    ~Flyweight()
    {
        delete shared_state_;
    }

    SharedState* shared_state() const
    {
        return shared_state_;
    }

    void Operation(const UniqueState& unique_state) const
    {
        std::cout << "Flyweight: Displaying shared (" << *shared_state_ << ") and unique (" << unique_state << ") state.\n";
    }

private:
    SharedState* shared_state_;

};

/**
 * @brief The FlyweightFactory creates and manages the Flyweight objects. It
 * ensures that flyweights are shared correctly. When the client requests a
 * flyweight, the factory either returns an existing instance or creates a new
 * one, if it doesn't exist yet.
 */
class FlyweightFactory
{
public:
    FlyweightFactory(std::initializer_list<SharedState> shared_states)
    {
        for (const SharedState& ss : shared_states)
        {
            this->flyweights_.insert(std::make_pair(this->GetKey(ss), Flyweight(&ss)));
        }
    }

    Flyweight GetFlyweight(const SharedState& shared_state)
    {
        std::string key = this->GetKey(shared_state);
        if (this->flyweights_.find(key) == flyweights_.end())
        {
            std::cout << "FlyweightFactory: Can't find a flyweight, creating a new one.\n";
            this->flyweights_.insert(std::make_pair(key, Flyweight(&shared_state)));
        }
        else
        {
            std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
        }

        return this->flyweights_.at(key);
    }

    void ListFlyweights() const
    {
        size_t count = this->flyweights_.size();
        std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
        for (std::pair<std::string, Flyweight> pair : flyweights_)
        {
            std::cout << pair.first << "\n";
        }
    }

private:
    std::unordered_map<std::string, Flyweight> flyweights_;

    std::string GetKey(const SharedState& ss) const
    {
        return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
    }
};

/**
 * @brief The Client has to provide the extrinsic state via arguments.
 * The Flyweight object should be acquired using the Factory.
 * 
 * NOTE: The Client code usually creates a bunch of pre-populated flyweights
 * in the initialization stage of the application.
 */
void AddCarToPoliceDatabase(FlyweightFactory& ff, const std::string& plates,
    const std::string& owner, const std::string& brand,
    const std::string& model, const std::string& color)
{
    std::cout << "\nClient: Adding a car to database.\n";
    const Flyweight& flyweight = ff.GetFlyweight({brand, model, color});

    flyweight.Operation({owner, plates});
}

int main()
{
    FlyweightFactory* factory = new FlyweightFactory({
        { "Chevrolet", "Camaro", "pink" },
        { "Mercedes Benz", "C300", "black" },
        { "Mercedes Benz", "C500", "red" },
        { "BMW", "M5", "red" },
        { "BMW", "X6", "white" } });

    factory->ListFlyweights();

    AddCarToPoliceDatabase(
        *factory,
        "CL234IR",
        "James Doe",
        "BMW",
        "M5",
        "red");

    AddCarToPoliceDatabase(
        *factory,
        "CL234IR",
        "James Doe",
        "BMW",
        "X1",
        "red");

    factory->ListFlyweights();
    delete factory;

    return 0;
}

// Output:
// FlyweightFactory: I have 5 flyweights:
// Chevrolet_Camaro_pink
// Mercedes Benz_C300_black
// Mercedes Benz_C500_red
// BMW_X6_white
// BMW_M5_red

// Client: Adding a car to database.
// FlyweightFactory: Reusing existing flyweight.
// Flyweight: Displaying shared ([ BMW, M5, red ]) and unique ([ James Doe, CL234IR ]) state.

// Client: Adding a car to database.
// FlyweightFactory: Can't find a flyweight, creating a new one.
// Flyweight: Displaying shared ([ BMW, X1, red ]) and unique ([ James Doe, CL234IR ]) state.

// FlyweightFactory: I have 6 flyweights:
// BMW_X1_red
// Chevrolet_Camaro_pink
// Mercedes Benz_C300_black
// Mercedes Benz_C500_red
// BMW_X6_white
// BMW_M5_red