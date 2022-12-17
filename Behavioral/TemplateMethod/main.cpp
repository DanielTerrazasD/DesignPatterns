#include <iostream>
#include <string>

/**
 * @brief The AbstractClass defines a template method that contains a skeleton
 * of some algorithm, composed of calls to (usually) abstract primitive 
 * operations.
 * 
 * Concrete subclasses should implement these operations, but leave the
 * template method itself intact.
 */
class AbstractClass
{
public:
    void TemplateMethod() const
    {
        this->BaseOperation1();
        this->RequiredOperation1();
        this->BaseOperation2();
        this->Hook1();
        this->RequiredOperation2();
        this->BaseOperation3();
        this->Hook2();
    }

protected:
    void BaseOperation1() const
    {
        std::cout << "AbstractClass says: I'm doing the bulk of the work.\n";
    }

    void BaseOperation2() const
    {
        std::cout << "AbstractClass says: But I let subclasses override some operations.\n";
    }

    void BaseOperation3() const
    {
        std::cout << "AbstractClass says: But I'm doing the bulk of the work anyway.\n";
    }

    virtual void RequiredOperation1() const = 0;
    virtual void RequiredOperation2() const = 0;

    /**
     * NOTE: These are "hooks". Subclasses may override them, but it's not
     * mandatory since the hooks already have default (but empty)
     * implementation. Hooks provide additional extension points in some
     * crucial places of the algorithm.
     */
    virtual void Hook1() const {}
    virtual void Hook2() const {}
};

/**
 * @brief Concrete classes have to implement all abstract operations of the
 * base class. They can also override some operations with a default
 * implementation.
 * 
 * Usually, concrete classes override only a fraction of base class'
 * operations.
 */
class ConcreteClass1 : public AbstractClass
{
protected:
    void RequiredOperation1() const override
    {
        std::cout << "ConcreteClass1 says: Implemented Operation1.\n";
    }

    void RequiredOperation2() const override
    {
        std::cout << "ConcreteClass1 says: Implemented Operation2.\n";
    }
};

class ConcreteClass2 : public AbstractClass
{
protected:
    void RequiredOperation1() const override
    {
        std::cout << "ConcreteClass2 says: Implemented Operation1.\n";
    }

    void RequiredOperation2() const override
    {
        std::cout << "ConcreteClass2 says: Implemented Operation2.\n";
    }

    void Hook1() const override
    {
        std::cout << "ConcreteClass2 says: Overriden Hook1.\n";
    }
};

/**
 * @brief The ClientCode calls the template method to execute the algorithm.
 * Client code does not have to know the concrete class of an object it works
 * with, as long as it works with objects through the interface of their base
 * class.
 */
void ClientCode(AbstractClass* class_)
{
    // ...
    class_->TemplateMethod();
    // ...
}

int main()
{
    std::cout << "Same Client Code can work with different subclasses:\n";
    ConcreteClass1* concreteClass1 = new ConcreteClass1;
    ClientCode(concreteClass1);
    std::cout << "\n";
    std::cout << "Same Client Code can work with different subclasses:\n";
    ConcreteClass2* concreteClass2 = new ConcreteClass2;
    ClientCode(concreteClass2);

    delete concreteClass1;
    delete concreteClass2;
    return 0;
}

// Output:
// Same Client Code can work with different subclasses:
// AbstractClass says: I'm doing the bulk of the work.
// ConcreteClass1 says: Implemented Operation1.
// AbstractClass says: But I let subclasses override some operations.
// ConcreteClass1 says: Implemented Operation2.
// AbstractClass says: But I'm doing the bulk of the work anyway.

// Same Client Code can work with different subclasses:
// AbstractClass says: I'm doing the bulk of the work.
// ConcreteClass2 says: Implemented Operation1.
// AbstractClass says: But I let subclasses override some operations.
// ConcreteClass2 says: Overriden Hook1.
// ConcreteClass2 says: Implemented Operation2.
// AbstractClass says: But I'm doing the bulk of the work anyway.