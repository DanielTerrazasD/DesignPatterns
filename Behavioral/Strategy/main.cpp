#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

/**
 * @brief Strategy interface declares operations common to all supported
 * versions of some algorithm.
 * 
 * The Context uses this interface to call the algorithm defined by Concrete
 * Strategies.
 */
class Strategy
{
public:
    virtual ~Strategy() = default;
    virtual std::string DoAlgorithm(std::string_view data) const = 0;
};

/**
 * @brief The Context defines the interface of interest to clients.
 * Usually, the Context accepts a strategy through the constructor, but also
 * provides a setter to change it at runtime.
 */
class Context
{
public:
    explicit Context(std::unique_ptr<Strategy>&& strategy = {}) : strategy_(std::move(strategy)) {}

    void SetStrategy(std::unique_ptr<Strategy>&& strategy)
    {
        strategy_ = std::move(strategy);
    }

    void DoSomeBusinessLogic() const
    {
        if (strategy_)
        {
            std::cout << "Context: Sorting data using the strategy (not sure how it'll do it)\n";
            std::string result = strategy_->DoAlgorithm("aecbd");
            std::cout << result << "\n";
        }
        else
            std::cout << "Context: Strategy isn't set\n";
    }

private:
    /**
     * NOTE: The Context maintains a reference to one of the Strategy objects.
     * The Context does not know the concrete class of a strategy. It should
     * work with all strategies via the Strategy interface.
     */
    std::unique_ptr<Strategy> strategy_;
};

/**
 * @brief Concrete Strategies implement the algorithm while following the base
 * Strategy interface. The interface makes them interchangeable in the Context.
 */
class ConcreteStrategyA : public Strategy
{
public:
    std::string DoAlgorithm(std::string_view data) const override
    {
        std::string result(data);
        std::sort(std::begin(result), std::end(result));
        return result;
    }
};

class ConcreteStrategyB : public Strategy
{
public:
    std::string DoAlgorithm(std::string_view data) const override
    {
        std::string result(data);
        std::sort(std::begin(result), std::end(result), std::greater<>());
        return result;
    }
};

/**
 * @brief The Client Code picks a concrete strategy and passes it to the
 * context. The client should be aware of the differences between strategies
 * in order to make the right choice.
 */
void ClientCode()
{
    Context context(std::make_unique<ConcreteStrategyA>());
    std::cout << "Client: Strategy is set to normal sorting.\n";
    context.DoSomeBusinessLogic();
    std::cout << "\n";
    std::cout << "Client: Strategy is set to reverse sorting.\n";
    context.SetStrategy(std::make_unique<ConcreteStrategyB>());
    context.DoSomeBusinessLogic();
}

int main()
{
    ClientCode();
    return 0;
}

// Output:
// Client: Strategy is set to normal sorting.
// Context: Sorting data using the strategy (not sure how it'll do it)
// abcde

// Client: Strategy is set to reverse sorting.
// Context: Sorting data using the strategy (not sure how it'll do it)
// edcba