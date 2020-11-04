#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;
using namespace Catch::Matchers;

TEST_CASE("references vs. pointers")
{
    int value = 42;
    int another_value = 102;

    SECTION("pointers")
    {
        int* ptr_value = nullptr;
            
        ptr_value = &value;

        if (ptr_value)
            *ptr_value = 665;

        std::cout << ptr_value << " - " << *ptr_value << "\n";

        REQUIRE(value == 665);

        ptr_value = &another_value;

        REQUIRE(*ptr_value == 102);
    }

    SECTION("reference")
    {
        int& ref_value = value; 

        ref_value = 665;

        std::cout << &ref_value << " - " << ref_value << "\n";

        REQUIRE(value == 665);
    }
}

template <typename T>
void print(const T& container)
{
    for (auto item : container)
        std::cout << item << " ";
    std::cout << "\n";
}

namespace InParam
{
	void foo(int x) // prymitive objects
	{
		    std::cout << "foo(int: " << x << ")\n";
	}

	void foo(const std::vector<int>& data) // custom types
	{
		    print(data);
	}

	void foo2(std::vector<int> data)
	{
		    data.push_back(4);

		    print(data);
	}		
}

void swap_pointers(int** ptr1, int** ptr2)
{
    int** temp = ptr1;
    *ptr1 = *ptr2;
    *ptr2 = *temp;
}

using IntPtr = int*;

void swap_pointers(IntPtr& ptr1, IntPtr& ptr2)
{
    int* temp = ptr1;
    ptr1 = ptr2;
    ptr2 = temp;
}

template <typename T>
void swap(T& ptr1, T& ptr2)
{
    T temp = ptr1;
    ptr1 = ptr2;
    ptr2 = temp;
}

namespace InOutParam
{
    void increment(int* ptr)
    {
        if (ptr)
            *ptr += 1;
    }

    void increment(int& value)
    {
        ++value;
    }
}

TEST_CASE("functions - passing arguments")
{
    int x = 42;
    InParam::foo(x); // passing by value

    std::vector<int> vec = { 1, 2, 3 };
    
    InParam::foo(vec); // passing by reference
    print(vec);

    InParam::foo2(std::move(vec)); // passing with move semantics

    InOutParam::increment(&x);
    REQUIRE(x == 43);

    InOutParam::increment(x);
    REQUIRE(x == 44);

    print(vec); // passing lvalue
    print(std::vector<int>{1, 2, 3, 4}); // passing temporary vector - rvalue
}

std::string full_name(const std::string& first, const std::string& last)
{
    return first + " " + last;
}

std::vector<int> create_vector(size_t size)
{
    std::vector<int> vec(size);

    std::iota(vec.begin(), vec.end(), 1);

    return vec;
}

decltype(auto) value()
{
    static int x = 42;
    int& ref_x = x;
    return ref_x;
}

TEST_CASE("returning values from functions")
{
    std::string person = full_name("Jan", "Kowalski");

    REQUIRE(person == "Jan Kowalski"s);

    std::vector<int> data = create_vector(1'000'000);

    int& ref_value = value();

    std::cout << ref_value << "\n";
}

template <typename T>
auto add(const T& x, const T& y)
{
    return x + y;
}

template <typename T>
auto multiply(const T& x, const T& y)
{
    return x * y;
}

auto describe(int x)
{
    if (x % 2 == 0)
        return std::string("Even");
    else
        return "Odd"s;
}

TEST_CASE("auto as return type")
{
    auto result = add(1, 5); // int result

    auto word = add("one"s, "two"s); // std::string word
}