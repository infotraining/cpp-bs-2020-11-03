#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <list>
#include <execution>

#include "catch.hpp"

using namespace std;

TEST_CASE("iterators in c")
{
	int tab[10] = { 1, 2, 3, 4, 5 };

	SECTION("iteration with pointer")
	{
		for (int* it = tab; it != tab + 10; ++it)
			*it *= 2;

		for (const int* cit = tab; cit != tab + 10; ++cit)
			std::cout << *cit << " ";
		std::cout << "\n";
	}
}


TEST_CASE("vector & iterators")
{
	std::vector<int> vec = { 1, 2, 3, 4 };

	SECTION("iterator - read-write")
	{
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			*it *= 2;
		}

		for (auto it = vec.begin(); it != vec.end(); ++it)
		{
			*it *= 2;
		}
	}

	SECTION("iterator - read-only")
	{
		for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
		{
			std::cout << *it << " ";
		}

		std::cout << "\n";

		for (auto it = vec.cbegin(); it != vec.cend(); ++it)
		{
			std::cout << *it << " ";
		}

		std::cout << "\n";
	}
}

TEST_CASE("range based for")
{
	std::vector<int> vec = { 1, 2, 3, 4 };

	for (const auto& item : vec)
	{
		std::cout << item << " ";
	}
	std::cout << "\n";

	SECTION("is interpreted as")
	{
		auto it = vec.begin();
		auto end = vec.end();
		for (; it != end; ++it)
		{
			const auto& item = *it;
		
			std::cout << item << " ";
		}
	}
}

template <typename T>
void print(const T& container, const std::string& desc)
{
	std::cout << desc << " : [ ";
	for (auto item : container)
		std::cout << item << " ";
	std::cout << "]\n";
}

TEST_CASE("iterators & vector")
{
	std::vector<int> vec = { 1, 2, 3, 4 };

	vec.insert(vec.begin() + 2, 5, 0);

	print(vec, "vec");

	vec.erase(vec.begin() + 3, vec.end() - 2);

	print(vec, "vec");
}

struct Person
{
	int id;
	std::string name;

	void print() const
	{
		std::cout << "Person(" << id << ", " << name << ")\n";
	}
};

TEST_CASE("vector of objects & iterators")
{
	std::vector<Person> people = { Person{1, "Jan"}, Person{2, "Ewa"}, Person{3, "Adam"} };

	for (auto it = people.cbegin(); it != people.cend(); ++it)
	{
		it->print();
	}
}

template <typename Iterator, typename T>
Iterator my_find(Iterator first, Iterator last, const T& value)
{
	for (auto it = first; it != last; ++it)
	{
		if (*it == value)
			return it;
	}

	return last;
}

template <typename Iterator, typename F>
Iterator my_find_if(Iterator first, Iterator last, F predicate)
{
	for (auto it = first; it != last; ++it)
	{
		if (predicate(*it))
			return it;
	}

	return last;
}

bool is_even(int x)
{
	return x % 2 == 0;
}

TEST_CASE("find")
{
	std::vector<int> numbers = { 1, 5, 8, 42, 77, 665 };

	SECTION("finding value")
	{
		auto pos = std::find(std::begin(numbers), std::end(numbers), 42);

		if (pos != std::end(numbers))
		{
			std::cout << *pos << " has been found...\n";
			std::cout << "Index: " << std::distance(std::begin(numbers), pos) << "\n";
		}
		else
		{
			std::cout << "Item has not been found...\n";
		}

		REQUIRE(std::count(std::begin(numbers), std::end(numbers), 42) == 1);
	}

	SECTION("finding with predicate")
	{
		auto pos = std::find_if(std::begin(numbers), std::end(numbers), &is_even);

		if (pos != std::end(numbers))
		{
			std::cout << *pos << " is even & has been found...\n";
		}
		else
		{
			std::cout << "Item has not been found...\n";
		}

		REQUIRE(std::count_if(std::begin(numbers), std::end(numbers), [](int x) { return x % 2 == 0; }) == 2);
	}
}

TEST_CASE("algorithms & lambda")
{
	std::vector<Person> people = { Person{1, "Jan"}, Person{2, "Ewa"}, Person{3, "Adam"} };

	
	auto pos = std::find_if(std::execution::par, people.begin(), people.end(), [](const Person& p) { return p.name == "Ewa"s; });

	if (pos != people.end())
	{
		std::cout << "Found: ";
		pos->print();
	}
}