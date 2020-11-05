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

class Array
{
private:
	size_t size_;
	int* data_;
public:
	typedef int* iterator; // legacy style
	using const_iterator = const int*; // since C++11

	Array(size_t size, int value = 0)
		: size_(size), data_(new int[size])
	{	
		std::fill_n(data_, size_, value);
	}

	// allows list initialization: Array a = {1, 2, 3}
	Array(std::initializer_list<int> il)
		: size_(il.size()), data_(new int[il.size()])
	{
		std::copy(il.begin(), il.end(), data_);
	}

	// copy constructor
	Array(const Array& source) : size_(source.size_), data_(new int[source.size_])
	{
		std::copy(source.begin(), source.end(), this->data_);
	}

	// copy assignment operator
	Array& operator=(const Array& source)
	{
		if (this != &source) // protection from self-assignment
		{
			delete[] data_; // free memory

			// copy of state from source object
			size_ = source.size_;
			data_ = new int[size_];
			std::copy(source.begin(), source.end(), data_);
		}

		return *this;
	}

	// destructor
	~Array()
	{
		delete[] data_;
	}

	iterator begin()
	{
		return data_;
	}

	const_iterator begin() const
	{
		return data_;
	}

	iterator end()
	{
		return data_ + size_;
	}

	const_iterator end() const
	{
		return data_ + size_;
	}

	void reset(int value)
	{
		std::fill_n(data_, size_, value);
	}

	size_t size() const
	{
		return this->size_;
	}

	int& operator[](size_t index)
	{
		return data_[index];
	}

	const int& operator[](size_t index) const
	{
		return data_[index];
	}
};

bool operator==(const Array& lhs, const Array& rhs)
{
	/*if (lhs.size() != rhs.size())
		return false;

	bool are_same = true;
	for (size_t i = 0; i < lhs.size(); ++i)
	{
		if (lhs[i] != rhs[i])
		{
			are_same = false;
			break;
		}
	}

	return are_same;*/

	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

TEST_CASE("Array - copy & assign")
{
	Array arr1 = { 1, 2, 3, 4 };

	SECTION("copy constructor")
	{
		Array arr2 = arr1; // copy constuctor

		CHECK(arr2 == Array{ 1, 2, 3, 4 });
	}

	SECTION("copy assingment")
	{
		Array arr3(3);
		CHECK(arr3 == Array{ 0, 0, 0 });

		arr3 = arr1; // copy assignment
		CHECK(arr3 == Array{ 1, 2, 3, 4 });

		arr3 = arr3; // self assignment
	}
}

TEST_CASE("Array - construction with size, default value")
{
	Array arr1(10);

	SECTION("size is set")
	{
		REQUIRE(arr1.size() == 10);
	}

	SECTION("all items are zero")
	{
		for (size_t i = 0; i < arr1.size(); ++i)
			CHECK(arr1[i] == 0);
	}
}

TEST_CASE("Array - construction with size & value")
{
	Array arr1(10, 1);

	SECTION("size is set")
	{
		REQUIRE(arr1.size() == 10);
	}

	SECTION("all items are zero")
	{
		for (size_t i = 0; i < arr1.size(); ++i)
			CHECK(arr1[i] == 1);
	}
}

TEST_CASE("Array - construction with list")
{
	const Array arr1 = { 1, 2, 3 };

	CHECK(arr1.size() == 3);

	CHECK(arr1[0] == 1);
	CHECK(arr1[1] == 2);
	CHECK(arr1[2] == 3);
}

TEST_CASE("Indexing")
{
	Array arr1(10);

	arr1[0] = 1;
	arr1[5] = 6;

	CHECK(arr1[0] == 1);
	CHECK(arr1[5] == 6);
}

TEST_CASE("Reset")
{
	Array arr1 = { 1, 2, 3 };

	arr1.reset(0);

	CHECK(arr1[0] == 0);
	CHECK(arr1[1] == 0);
	CHECK(arr1[2] == 0);
}

TEST_CASE("For-each")
{
	Array arr1(10);

	for (auto& item : arr1)
		item = 1;

	REQUIRE(std::all_of(arr1.begin(), arr1.end(), [](int x) { return x == 1; }));

	//SECTION("const object & iterators")
	//{
	//	const Array carr = { 1, 2, 3 };
	//	for (const auto& item : carr)
	//	{
	//		std::cout << item << " ";
	//	}
	//	std::cout << "\n";

	//	Array::const_iterator it = carr.begin();
	//	CHECK(*it == 1);
	//}
}

class MultiArray
{
private:
	size_t size_;
	int* data1_;
	int* data2_;
public:
	MultiArray(size_t size)
		: size_(size), data1_(new int[size]), data2_(new int[size])
	{
		std::fill_n(data1_, size_, 0);
		std::fill_n(data2_, size_, 0);
	}

	size_t size() const
	{
		return size_;
	}

	std::tuple<int&, int&> operator[](size_t index)
	{
		return std::tie(data1_[index], data2_[index]);
	}
};

TEST_CASE("MultiArray")
{
	MultiArray ma(10);

	tuple<int, int> col = ma[0];
	CHECK(std::get<0>(col) == 0);
	CHECK(std::get<1>(col) == 0);

	ma[0] = tuple(1, 2);

	auto [firt_item, second_item] = ma[0];
	CHECK(firt_item == 1);
	CHECK(second_item == 2);
}

// value semantics
struct Data
{
	std::string name;
	std::vector<int> data;
	Array array;

	void print() const
	{
		std::cout << "Data(" << name << ", [ ";
		for (const auto& item : data)
			std::cout << item << " ";
		std::cout << "], [ ";
		for (const auto& item : array)
			std::cout << item << " ";
		std::cout << "]) - " << this << "\n";
	}
};

TEST_CASE("aggregate initialization")
{
	int tab1[10] = { 1, 2, 3 }; // aggregate initialization
	int tab2[10]{ 1, 2, 3 };    // aggregate initialization

	Data d1{ "d1", { 1, 2, 3 }, {4, 5, 6} }; // aggregate initialization

	Array arr1{ 1, 2, 3 }; // constructor with std::initializer_list<int>
}

TEST_CASE("copy & assign")
{
	Data d1{ "d1", { 1, 2, 3 }, {4, 5, 6 } }; // aggregate initialization

	Data d2 = d1; // copy construction
	Data d3(d1);  // copy construction
	Data d4{ d1 }; // copy construction

	d3.data[1] = 0;
	d3.name = "modified";
	d1.print();
	d3.print();

	d3 = d1; // copy assignment
	d3.print();
}