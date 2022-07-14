#include <iostream>
#include <vector>
#include <string>

// Design Pattern - Builder

/**
 * @brief Makes sense to use the Builder pattern only when your products are
 * quite complex and require extensive configuration.
 * 
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated products. In other words, results of various builders may not
 * always follow the same interface.
 */
class Product1
{
public:
    std::vector<std::string> parts_;

    void ListParts() const
    {
        std::cout << "Product parts: ";
        for (size_t i = 0; i < parts_.size(); i++)
        {
            if (parts_[i] == parts_.back())
                std::cout << parts_[i];
            else
                std::cout << parts_[i] << ", ";
        }

        std::cout << "\n\n";
    }
};

/**
 * @brief Builder interface specifies methods for creating the different parts
 * of the Product objects.
 */
class Builder
{
public:
    virtual ~Builder() {}
    // Production steps can be pure virtual or just virtual
    virtual void ProducePartA() const = 0;
    virtual void ProducePartB() const = 0;
    virtual void ProducePartC() const = 0;
};

/**
 * @brief Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have
 * several variations of Builders, implemented differently.
 */
class ConcreteBuilder1 : public Builder
{
    /**
     * A fresh builder instance should contain a blank product object, which
     * is used in further assembly.
     */
public:
    ConcreteBuilder1()
    {
        this->Reset();
    }

    ~ConcreteBuilder1()
    {
        delete product;
    }

    void Reset()
    {
        this->product = new Product1();
    }

    /**
     * All production steps work with the same product instance.
     */
    void ProducePartA() const override
    {
        this->product->parts_.push_back("PartA1");
    }

    void ProducePartB() const override
    {
        this->product->parts_.push_back("PartB1");
    }

    void ProducePartC() const override
    {
        this->product->parts_.push_back("PartC1");
    }

    /**
     * Concrete Builders are supposed to provide their own methods for
     * retrieving results. That's because various types of builders may create
     * entirely different products that don't follow the same interface.
     * Therefore, such methods cannot be declared in the base Builder interface
     * (at least in a statically typed programming language).
     */

    /**
     * Please be careful here with the memory ownership. Once you call
     * GetProduct, the user of this function is responsible to release this
     * memory. Here could be a better option to use smart pointers to avoid
     * memory leaks.
     */
    Product1* GetProduct()
    {
        Product1* result = this->product;
        this->Reset();
        return result;
    }

private:
    Product1* product;
};

/**
 * @brief Director is only responsible for executing the building steps in a
 * particular sequence. It is helpful when producing products according to a
 * specific order or configuration. Strictly speaking, the Director class is
 * optional, since the client can control builders directly.
 */
class Director
{
public:
    void SetBuilder(Builder* builder)
    {
        this->builder = builder;
    }

    /**
     * The Director can construct several product variations using the same
     * building steps.
     */
    void BuildMinimalViableProduct()
    {
        this->builder->ProducePartA();
    }

    void BuildFullFeaturedProduct()
    {
        this->builder->ProducePartA();
        this->builder->ProducePartB();
        this->builder->ProducePartC();
    }

private:
    Builder* builder;
};

/**
 * @brief Client code creates a builder object, passes it to the director and
 * then initiates the construction process. The end result is retrieved from
 * the builder object.
 */
void ClientCode(Director& director)
{
    /**
     * Raw pointers are used for simplicity, however you may prefer to use
     * smart pointers here.
     */

    ConcreteBuilder1* builder = new ConcreteBuilder1();
    director.SetBuilder(builder);
    std::cout << "Standard basic product:\n";
    director.BuildMinimalViableProduct();

    Product1* p = builder->GetProduct();
    p->ListParts();
    delete p;

    std::cout << "Standard full featured product:\n";
    director.BuildFullFeaturedProduct();

    p = builder->GetProduct();
    p->ListParts();
    delete p;

    // The Builder pattern can be used without a Director class.
    std::cout << "Custom product:\n";
    builder->ProducePartA();
    builder->ProducePartC();
    p = builder->GetProduct();
    p->ListParts();
    delete p;

    delete builder;
}

int main()
{
    Director* director = new Director();
    ClientCode(*director);
    delete director;

    return 0;
}

// Output:
//Standard basic product :
//Product parts : PartA1

//Standard full featured product :
//Product parts : PartA1, PartB1, PartC1

//Custom product :
//Product parts : PartA1, PartC1