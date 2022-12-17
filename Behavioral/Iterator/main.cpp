#include <iostream>
#include <string>
#include <vector>

/**
 * @brief C++ has its own implementation of iterator that works with a
 * different generics containers defined by the standard library.
 */
template <typename T, typename U>
class Iterator
{
public:
    typedef typename std::vector<T>::iterator iter_type;
    
    Iterator(U* p_data) : m_p_data_(p_data)
    {
        m_it_ = m_p_data_->m_data_.begin();
    }

    void First()
    {
        m_it_ = m_p_data_->m_data_.begin();
    }

    void Next()
    {
        m_it_++;
    }

    bool IsDone()
    {
        return (m_it_ == m_p_data_->m_data_.end());
    }

    iter_type Current()
    {
        return m_it_;
    }

private:
    U* m_p_data_;
    iter_type m_it_;
};

/**
 * @brief Generic Collections/Containers provide one or several methods for
 * retrieving fresh iterator instances, compatible with the collection class.
 */
template <class T>
class Container
{
    friend class Iterator<T, Container>;

public:
    void Add(T a)
    {
        m_data_.push_back(a);
    }

    Iterator<T, Container>* CreateIterator()
    {
        return new Iterator<T, Container>(this);
    }

private:
    std::vector<T> m_data_;
};

class Data
{
public:
    Data(int a = 0) : m_data_(a) {}

    void SetData(int a)
    {
        m_data_ = a;
    }

    int CurrentData() const
    {
        return m_data_;
    }

private:
    int m_data_;
};

/**
 * @brief The ClientCode may or may not know about the Concrete Iterator or
 * Collection classes, for this implementation the container is generi so it
 * can be used with an "int" or with custom class.
 */
void ClientCode()
{
    std::cout << "Iterator with (int):\n";
    Container<int> cont;

    for (int i = 0; i < 10; i++)
    {
        cont.Add(i);
    }

    Iterator<int, Container<int>>* it = cont.CreateIterator();
    for (it->First(); !it->IsDone(); it->Next())
    {
        std::cout << *it->Current() << "\n";
    }

    Container<Data> cont2;
    Data a(100), b(1000), c(10000);
    cont2.Add(a);
    cont2.Add(b);
    cont2.Add(c);

    std::cout << "Iterator with (custom Class):\n";
    Iterator<Data, Container<Data>>* it2 = cont2.CreateIterator();
    for (it2->First(); !it2->IsDone(); it2->Next())
    {
        std::cout << it2->Current()->CurrentData() << "\n";
    }

    delete it;
    delete it2;
}

int main()
{
    ClientCode();
    return 0;
}

// Output:
// Iterator with (int):
// 0
// 1
// 2
// 3
// 4
// 5
// 6
// 7
// 8
// 9
// Iterator with (custom Class):
// 100
// 1000
// 10000