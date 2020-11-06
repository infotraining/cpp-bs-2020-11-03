#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>

#include "catch.hpp"

using namespace std;

int add(int a , int b)
{
	static int counter = 0;
	++counter;

	return a + b;
}

inline int f(int x)
{
	return 2 * x;
}

using FunctionPtr = int (*)(int);


namespace Step1
{
	auto calculate(const std::vector<int>& vec, int (*f_param)(int))
	{
		std::vector<int> result = vec;

		for (auto& item : result)
			item = f_param(item);

		return result;
	}
}

template <typename Function>
auto calculate(const std::vector<int>& vec, Function f_param)
{
	std::vector<int> result = vec;

	for (auto& item : result)
		item = f_param(item);

	return result;
}

template <typename InIter, typename OutIter, typename Function>
OutIter my_transform(InIter first, InIter last, OutIter out, Function f)
{
	while (first != last)
	{
		*out = f(*first);
		++out;
		++first;
	}

	return out;
}

class Add
{
	int counter_ = 0;
public:
	int operator()(int a, int b)
	{
		++counter_;
		return a + b;
	}

	int counter() const
	{
		return counter_;
	}
};

class MultiplyBy
{
	int factor_;
public:
	MultiplyBy(int factor) : factor_(factor)
	{}

	int operator()(int a) const
	{
		return a * factor_;
	}
};

TEST_CASE("callable")
{
	int result = add(1, 2);
	REQUIRE(result == 3);

	SECTION("function pointer")
	{
		int (*ptr_fun)(int, int) = add;
		REQUIRE(ptr_fun(5, 6) == 11);

		std::vector<int> vec = { 1, 2, 3 };

		auto vec_result = calculate(vec, f);
		REQUIRE(vec_result == vector<int>{2, 4, 6});
	}

	SECTION("functor")
	{
		Add add_functor;
		int result = add_functor(1, 2);
		REQUIRE(result == 3);
		REQUIRE(add_functor.counter() == 1);

		result = add_functor(6, 7);
		REQUIRE(add_functor.counter() == 2);

		Add another_add;
		REQUIRE(another_add.counter() == 0);

		std::vector<int> vec = { 1, 2, 3 };
		auto vec_result = calculate(vec, MultiplyBy{3});
		REQUIRE(vec_result == vector<int>{3, 6, 9});

		int tab[] = { 1, 2, 3 };
		std::transform(std::begin(tab), std::end(tab), std::begin(tab), MultiplyBy(5));

		REQUIRE(tab[2] == 15);
	}
	
	/*
	class MagicLambda_764267345
	{
	public:
		int operator()(int a, int b) { return a + b; }
	};
	*/

	SECTION("lambda")
	{
		auto add_lambda = [](int a, int b) { return a + b; };

		//auto add_lambda = MagicLambda_764267345{};

		int factor = 5;
		
		int tab[] = { 1, 2, 3 };

		std::transform(std::begin(tab), std::end(tab), 
			std::begin(tab), [=](int a) { return a * factor; }); // [=] - capture by value
		REQUIRE(tab[2] == 15);

		int counter = 0;

		auto call = [&]() { ++counter; }; // [&] - capture by reference

		call();
		call();
		call();

		REQUIRE(counter == 3);
	}
}

template <typename T>
void print(const T& container)
{
	for (auto item : container)
		std::cout << item << " ";
	std::cout << "\n";
}

TEST_CASE("std algorithms")
{
	std::vector<std::string> words = { "zero", "sixty_six", "one", "two", "three", "four" };

	auto cmp_by_length = [](const std::string& a, const std::string& b) { return a.size() < b.size(); };

	std::sort(words.begin(), words.end(), cmp_by_length);

	std::vector<std::string> three_letters;
	three_letters.reserve(100);

	std::copy_if(std::begin(words), std::end(words), 
		std::back_inserter(three_letters), [](const std::string& w) { return w.size() == 3; });

	print(words);
	print(three_letters);
}