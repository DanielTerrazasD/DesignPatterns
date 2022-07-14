#include <iostream>
#include <string>

// Design Pattern - Factory Method

/**
 * @brief Product interface declares the operations that all concrete products
 * must implement.
 */
class Product
{
public:
    virtual ~Product() {}
    virtual std::string Operation() const = 0;
};

/**
 * @brief Concrete Products provide various implementations of the Product
 * interface.
 */
class ConcreteProduct1 : public Product
{
public:
    std::string Operation() const override
    {
        return "{Result of the ConcreteProduct1}";
    }
};

/**
 * @brief Concrete Products provide various implementations of the Product
 * interface.
 */
class ConcreteProduct2 : public Product
{
public:
    std::string Operation() const override
    {
        return "{Result of the ConcreteProduct2}";
    }
};

/**
 * @brief Creator class declares the factory method that is supposed to return
 * an object of a Product class. The Creator's subclasses usually provide
 * the implementation of this method.
 */
class Creator
{
    /**
     * The Creator may also provide some default implementation of the
     * factory method.
     */
public:
    virtual ~Creator() {};

    /**
     * Creator's primary responsability is not creating products.
     * Usually, it contains some core business logic that relies in Product
     * objects, returned by the factory method.
     * Subclasses can indirectly change that business logic by overriding the
     * factory method and return a different type of product from it.
     */
    virtual Product* FactoryMethod() const = 0;

    std::string SomeOperation() const
    {
        Product* product = this->FactoryMethod();
        std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
        delete product;
        return result;
    }
};

/**
 * @brief Concrete Creators override the factory method in order to change the
 * resulting product's type.
 */
class ConcreteCreator1 : public Creator
{
public:
    Product* FactoryMethod() const override
    {
        return new ConcreteProduct1();
    }
};

/**
 * @brief Concrete Creators override the factory method in order to change the
 * resulting product's type.
 */
class ConcreteCreator2 : public Creator
{
public:
    Product* FactoryMethod() const override
    {
        return new ConcreteProduct2();
    }
};

/**
 * @brief Client code works with an instance of a concrete creator, although
 * through its base interface. As long as the client keeps working with the
 * creator via the base interface, you can pass it any creator's subclass.
 * 
 */
void ClientCode(const Creator& creator)

{
    std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
        << creator.SomeOperation() << std::endl;

    // Client can also create a product using the FactoryMethod
    //Product* p = creator.FactoryMethod();
    //std::cout << p->Operation();
    //delete p;
}


/**
 * @brief The Application picks a creator's type depending on the configuration
 * or environment.
 */
int main()
{
    std::cout << "App: Launched with the ConcreteCreator1.\n";
    Creator* creator1 = new ConcreteCreator1();
    ClientCode(*creator1);
    std::cout << std::endl;

    std::cout << "App: Launched with the ConcreteCreator2.\n";
    Creator* creator2 = new ConcreteCreator2();
    ClientCode(*creator2);
    std::cout << std::endl;

    delete creator1;
    delete creator2;

    return 0;
}

// Output:
//App: Launched with the ConcreteCreator1.
//Client : I'm not aware of the creator's class, but it still works.
//Creator : The same creator's code has just worked with {Result of the ConcreteProduct1}

//App : Launched with the ConcreteCreator2.
//Client : I'm not aware of the creator's class, but it still works.
//Creator : The same creator's code has just worked with {Result of the ConcreteProduct2}