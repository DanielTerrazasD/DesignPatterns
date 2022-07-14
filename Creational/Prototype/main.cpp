#include <iostream>
#include <string>
#include <unordered_map>

// Design Pattern - Prototype

enum Type
{
    PROTOTYPE_1 = 0,
    PROTOTYPE_2,
};

/**
 * @brief Prototype class with cloning ability.
 */
class Prototype
{
public:
    Prototype() : prototype_name_(), prototype_field_() {}
    Prototype(std::string prototype_name) : prototype_name_(prototype_name), prototype_field_() {}

    virtual ~Prototype() {}
    virtual Prototype* Clone() const = 0;
    virtual void Method(float prototype_field)
    {
        this->prototype_field_ = prototype_field;
        std::cout << "Call Method from " << prototype_name_ << "with field: " << prototype_field_ << std::endl;
    }

protected:
    std::string prototype_name_;
    float prototype_field_;
};

/**
 * @brief Concrete prototypes implement the Clone method.
 * All data memberes of the Prototype class are in the Stack.
 * If you have pointers in your properties (e.g. string* name), you will need
 * to implement the copy-constructor to make sure you deep-copy from the Clone
 * method.
 */
class ConcretePrototype1 : public Prototype
{
public:
    ConcretePrototype1(std::string prototype_name, float concrete_prototype_field)
        : Prototype(prototype_name), concrete_prototype_field1_(concrete_prototype_field) {}

    /**
     * Notice that Clone method return a raw pointer to a new ConcretePrototype
     * replica. So, the client has the responsibility to free that memory.
     * Here could be a better option to use smart pointers to avoid memory
     * leaks.
     */
    Prototype* Clone() const override
    {
        return new ConcretePrototype1(*this);
    }
private:
    float concrete_prototype_field1_;
};

/**
 * @brief Concrete prototypes implement the Clone method.
 * All data memberes of the Prototype class are in the Stack.
 * If you have pointers in your properties (e.g. string* name), you will need
 * to implement the copy-constructor to make sure you deep-copy from the Clone
 * method.
 */
class ConcretePrototype2 : public Prototype
{
public:
    ConcretePrototype2(std::string prototype_name, float concrete_prototype_field)
        : Prototype(prototype_name), concrete_prototype_field2_(concrete_prototype_field) {}

    Prototype* Clone() const override
    {
        // Note: Beware of "Shallow Copy" when using the default copy-constructor.
        return new ConcretePrototype2(*this);
    }
private:
    float concrete_prototype_field2_;
};

/**
 * @brief Prototype Factory class may store default concrete prototypes, one
 * for each concrete prototype class, so each time you want to create a bullet,
 * you can use the existing ones and clone those.
 */
class PrototypeFactory
{
public:
    PrototypeFactory()
    {
        prototypes_[Type::PROTOTYPE_1] = new ConcretePrototype1("PROTOTYPE_1 ", 50.0f);
        prototypes_[Type::PROTOTYPE_2] = new ConcretePrototype2("PROTOTYPE_2 ", 60.0f);
    }

    ~PrototypeFactory()
    {
        /**
         * Be carefull to free all the memory allocated for the default
         * prototypes. You may want to use smart pointers again.
         */
        delete prototypes_[Type::PROTOTYPE_1];
        delete prototypes_[Type::PROTOTYPE_2];
    }

    Prototype* CreatePrototype(Type type)
    {
        return prototypes_[type]->Clone();
    }

private:
    std::unordered_map<Type, Prototype*, std::hash<int>> prototypes_;
};

void ClientCode(PrototypeFactory& prototype_factory)
{
    std::cout << "Let's create a Prototype 1" << std::endl;
    Prototype* prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
    prototype->Method(90.0f);
    delete prototype;
    std::cout << std::endl;

    std::cout << "Let's create a Prototype 2" << std::endl;
    prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
    prototype->Method(10.0f);
    delete prototype;
    std::cout << std::endl;
}

int main()
{
    PrototypeFactory* prototype_factory = new PrototypeFactory();
    ClientCode(*prototype_factory);
    delete prototype_factory;
    return 0;
}

// Output:
//Let's create a Prototype 1
//Call Method from PROTOTYPE_1 with field : 90

//Let's create a Prototype 2
//Call Method from PROTOTYPE_2 with field : 10