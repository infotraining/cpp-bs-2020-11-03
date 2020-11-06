#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>

#include "catch.hpp"

using namespace std;

template <typename T>
class Array
{
private:
	size_t size_;
	T* data_;
public:
	typedef T* iterator; // legacy style
	using const_iterator = const T*; // since C++11

	Array(size_t size = 0, T value = T{})
		: size_(size), data_(new T[size])
	{
		std::fill_n(data_, size_, value);
	}

	// allows list initialization: Array a = {1, 2, 3}
	Array(std::initializer_list<T> il)
		: size_(il.size()), data_(new T[il.size()])
	{
		std::copy(il.begin(), il.end(), data_);
	}

	// copy constructor
	Array(const Array& source) : size_(source.size_), data_(new T[source.size_])
	{
		std::copy(source.begin(), source.end(), this->data_);
		std::cout << "Array(const Array& - copy constructor)\n";
	}

	// copy assignment operator
	Array& operator=(const Array& source)
	{
		if (this != &source) // protection from self-assignment
		{
			delete[] data_; // free memory

			// copy of state from source object
			size_ = source.size_;
			data_ = new T[size_];
			std::copy(source.begin(), source.end(), data_);
		}

		return *this;
	}

	// move constructor
	Array(Array&& source) : size_(source.size_), data_(source.data_)
	{
		source.data_ = nullptr;
		source.size_ = 0;
		std::cout << "Array(Array&& - move constructor)\n";
	}

	// move assignment operator
	Array& operator=(Array&& source)
	{
		if (this != &source) // protection from self-assignment
		{
			delete[] data_; // free memory

			// move state from the source object
			size_ = source.size_;
			data_ = source.data_;
			source.data_ = nullptr;
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

	T& operator[](size_t index)
	{
		return data_[index];
	}

	const T& operator[](size_t index) const
	{
		return data_[index];
	}
};

template <typename T>
bool operator==(const Array<T>& lhs, const Array<T>& rhs)
{
	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
void print(const T& container)
{
	for (auto item : container)
		std::cout << item << " ";
	std::cout << "\n";
}

template <typename T>
struct RGB
{
	T r, g, b;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const RGB<T>& pixel)
{
	out << "RGB(" << pixel.r << ", " << pixel.g << ", " << pixel.b << ")";

	return out;
}

TEST_CASE("templates")
{
	Array<double> arr1 = { 1.0, 2.71, 3.14, 4.0 };
	print(arr1);

	Array<std::string> arr2 = { "one", "two", "three" };
	print(arr2);

	Array<Array<RGB<uint32_t>>> pixels = { { {0, 0, 0}, {255, 255, 255} }, { {255, 255, 255}, {0, 0, 0} } };
	
	pixels[0][1] = RGB<uint32_t>{ 128, 0, 255 };

	print(pixels[0]);
	print(pixels[1]);
}

struct Data
{
	int id;
	std::vector<int> data;

	Data(int id) : id(id)
	{}
};

TEST_CASE("arrays & default constructor")
{
	/*Data arr1[] = { Data{0}, }

	Data* arr2 = new Data[10];*/

	Data d1{ 1 };
	REQUIRE(d1.data.size() == 0);

	std::vector<Data> vec;
	vec.push_back(Data{ 1 });
}