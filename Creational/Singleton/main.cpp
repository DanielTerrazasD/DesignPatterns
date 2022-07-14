#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

// Design Pattern - Singleton

/**
 * @brief Singleton class defines the GetInstance method that serves as an
 * alternative to constructor and lets clients access the same instance of
 * this class over and over.
 */
class Singleton
{
public:

    // Singletons should not be cloneable.
    Singleton(Singleton& other) = delete;
    // Singletons should not be assignable.
    void operator=(const Singleton&) = delete;

    /**
     * @brief GetInstance static method controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client
     * existing object stored in the static field. A concept called "Slow
     * Initialization".
     */
    static Singleton* GetInstance(std::string value);

    /**
     * Any singleton should define some business logic, which can be executed
     * on its instance.
     */
    void SomeBusinessLogic()
    {
        // ...
    }

    std::string GetValue() const
    {
        return value_;
    }

protected:
    /**
     * The Singleton's constructor and destructor should always be private to
     * prevent direct construction, destruction calls with the 'new' and
     * 'delete' operators.
     */

    Singleton(const std::string value) : value_(value) {}
    ~Singleton() {}
    std::string value_;

private:
    static Singleton* pinstance_;
    static std::mutex mutex_;

};

Singleton* Singleton::pinstance_ = nullptr;
std::mutex Singleton::mutex_{};

/**
 * @brief The first time GetInstance is called, the storage location will be
 * locked to make sure that the variable is null, then the value is set.
 * The lock is release once the control leaves the scope in which the lock
 * object was created.
 */
Singleton* Singleton::GetInstance(std::string value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Singleton(value);
    }

    return pinstance_;
}

void ThreadFoo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton* singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->GetValue() << "\n";
}

void ThreadBar()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton* singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->GetValue() << "\n";
}

int main()
{

    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);

    t1.join();
    t2.join();

    return 0;
}

// Possible output:
// BAR
// BAR