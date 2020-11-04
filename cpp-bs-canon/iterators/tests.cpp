#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <list>
#include <execution>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>

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

TEST_CASE("std::vector")
{
	std::vector<int> vec = { 1, 2, 3, 4 };

	REQUIRE(vec.size() == 4);
	REQUIRE(vec.capacity() == 4);

	vec.push_back(5);

	REQUIRE(vec.size() == 5);
	REQUIRE(vec.capacity() == 6);

	vec.reserve(1000);

	vec.clear();
	vec.shrink_to_fit();

	size_t prev_capacity = vec.capacity();

	for (int i = 0; i < 995; ++i)
	{
		vec.push_back(i);
		if (prev_capacity != vec.capacity())
		{
			std::cout << "Realocation to: " << vec.data() << " - " << vec.capacity() << "\n";
			prev_capacity = vec.capacity();
		} 
	}

	SECTION("random acces iterator")
	{
		auto it = vec.begin();

		it += 10; 

		auto dist = it - vec.begin();

		REQUIRE(dist == 10);
	}

	SECTION("invalidating pointers, iterators & refs")
	{
		std::vector<std::string> words = { "one", "two", "three" };
		words.reserve(10);

		std::string* ptr = &words[0];
		auto it = words.begin() + 1;
		std::string& ref = words[2];

		words.push_back("evil"); // no-reallocation

		std::cout << *ptr << " " << *it << " " << ref << "\n";
	}


	SECTION("sorting")
	{
		vector<int> vec = { 6, 346, 23465, 32, 665, 235, 3 };
		std::sort(vec.begin(), vec.end());
	}
}

TEST_CASE("list")
{
	std::list<int> lst = { 1, 2, 3 };

	lst.push_back(5);
	lst.push_front(0);
	lst.insert(lst.end(), { 6, 7, 8, 9, 0, 8, 234, 2, 2, 534 });

	SECTION("bidirectional iterator")
	{
		auto it = lst.begin();

		++it;
		--it;

		std::advance(it, 10);

		auto dist = std::distance(lst.begin(), it);

		REQUIRE(dist == 10);
	}

	lst.sort();
	lst.reverse();

	print(lst, "lst");
}

TEST_CASE("deque - double ended queue")
{
	std::deque<int> dq;

	dq.push_back(1);
	dq.push_back(2);
	dq.push_back(3);
	dq.push_front(0);
	dq.push_front(1);

	dq[1] = 9;
	dq[2] = dq[0];

	SECTION("random acces iterator")
	{
		auto it = dq.begin();

		it += 3;

		auto dist = it - dq.begin();

		REQUIRE(dist == 3);
	}
}

TEST_CASE("set")
{
	std::set<int> unique_numbers = { 1, 5, 8 };

	unique_numbers.insert(2);
	unique_numbers.insert(88);
	unique_numbers.insert(42);
	unique_numbers.insert(665);

	auto [pos, was_inserted] = unique_numbers.insert(42);

	REQUIRE_FALSE(was_inserted);

	print(unique_numbers, "unique_numbers");

	SECTION("find, insert, erase - logarithmic complexity - O(log(N))")
	{
		if (auto pos = unique_numbers.find(665); pos != unique_numbers.end())
		{
			std::cout << "Found: " << *pos << "\n";
		}

		unique_numbers.erase(42);
	}

	SECTION("multiset")
	{
		std::multiset<int, std::greater<>> ms(unique_numbers.begin(), unique_numbers.end());

		ms.insert(42);
		ms.insert(42);

		print(ms, "ms");

		for (auto it = ms.lower_bound(42); it != ms.upper_bound(42); ++it)
		{
			std::cout << "Iterating: " <<  *it << "\n";
		}
	}
}

TEST_CASE("maps")
{
	std::map<int, std::string> dict = { {1, "one"}, {3, "three"}, {2, "two"} };

	dict.insert(make_pair(4, "four"));
	dict.emplace(9, "nine");

	REQUIRE(dict[2] == "two"s);

	REQUIRE(dict[0] == ""s);

	REQUIRE_THROWS_AS(dict.at(8), std::out_of_range);

	dict[7] = "seven"; // not efficient when there is no 7 

	dict.insert_or_assign(6, "six"); // efficient

	SECTION("c++17")
	{
		for (const auto& [key, value] : dict) // structured bindings
		{
			std::cout << key << " - " << value << "\n";
		}
	}

	SECTION("before c++17")
	{
		for (const auto& kv : dict)
		{
			std::cout << kv.first << " - " << kv.second << "\n";
		}
	}	

	SECTION("multimap")
	{
		std::multimap<int, std::string> mdict(dict.begin(), dict.end());

		mdict.emplace(1, "jeden");
		mdict.emplace(1, "un");

		for (auto it = mdict.lower_bound(1); it != mdict.upper_bound(1); ++it)
		{
			std::cout << it->first << " - " << it->second << "\n";
		}
	}
}

TEST_CASE("unordered_map - O(1)")
{
	std::unordered_map<int, std::string> udict = { {1, "one"}, {3, "three"}, {2, "two"} };

	REQUIRE(udict[1] == "one"s);
}