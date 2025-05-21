#include <iostream>
#include <cassert>
#include <utility>

template <typename Derived>
class less_than_comparable {
public:
    friend bool operator>(const Derived& lhs, const Derived& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const Derived& lhs, const Derived& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>=(const Derived& lhs, const Derived& rhs) {
        return !(lhs < rhs);
    }

    friend bool operator==(const Derived& lhs, const Derived& rhs) {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    friend bool operator!=(const Derived& lhs, const Derived& rhs) {
        return (lhs < rhs) || (rhs < lhs);
    }
};

class Number : public less_than_comparable<Number> {
public:
    explicit Number(int value) : m_value{value} {
        counter++;
    }

    Number(const Number& other) : m_value{other.m_value} {
        counter++;
    }

    Number(Number&& other) noexcept : m_value{other.m_value} {
        counter++;
    }

    ~Number() {
        counter--;
    }

    int value() const { return m_value; }

    bool operator<(const Number& other) const {
        return m_value < other.m_value;
    }

    static size_t count_instances() {
        return counter;
    }

private:
    int m_value;
    inline static size_t counter = 0;
};


int main() {
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};

    std::cout << "Count: " << Number::count_instances() << std::endl;

    Number five{5};
    std::cout << "Count: " << Number::count_instances() << std::endl;
    
    assert(one >= one);
    assert(three <= four);
    assert(three > one);
    assert(one < four);

    return 0;
}