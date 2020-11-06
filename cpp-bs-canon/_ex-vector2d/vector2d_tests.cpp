#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "catch.hpp"

using namespace std;

namespace Vectors
{
	class Vector2D
	{
		double x_;
		double y_;

		static const Vector2D unit_x_;
		static const Vector2D unit_y_;

	public:
		explicit Vector2D(double x = 0.0, double y = 0.0)
			: x_(x), y_(y)
		{
		}

		double x() const { return x_; }

		double y() const { return y_; }

		double length() const { return sqrt(x_ * x_ + y_ * y_); }
		
		static const Vector2D& unit_x()
		{
			return unit_x_;

		}

		static const Vector2D& unit_y()
		{
			return unit_y_;

		}

		Vector2D operator*(double value) const
		{
			return Vector2D(x() * value, y() * value);
		}

		//Vector2D& operator*=(double value)
		//{
		//	x_ *= value;
		//	y_ *= value;
		//	
		//	return *this;
		//}

		friend Vector2D& operator*=(Vector2D& v, double value);
		friend std::istream& operator>>(std::istream& in, Vectors::Vector2D& vec);
	};

	const Vector2D Vector2D::unit_x_(1.0, 0.0);
	const Vector2D Vector2D::unit_y_(0.0, 1.0);

	Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
	{
		return Vector2D(v1.x() + v2.x(), v1.y() + v2.y());
	}

	Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
	{
		return Vector2D(v1.x() - v2.x(), v1.y() - v2.y());
	}

	Vector2D operator-(const Vector2D& v1)
	{
		return Vector2D(-v1.x(), -v1.y());
	}

	bool operator==(const Vector2D& v1, const Vector2D& v2)
	{
		return ((v1.x() == v2.x()) && (v1.y() == v2.y()));
	}
	
	bool operator!=(const Vector2D& v1, const Vector2D& v2)
	{
		return !(v1 == v2);
	}

	Vector2D operator*(double value, const Vector2D& v1)
	{
		return Vector2D(v1.x() * value, v1.y() * value);
	}

	double operator*(const Vector2D& v1, const Vector2D& v2)
	{
		return v1.x() * v2.x() + v1.y() * v2.y();
	}

	Vector2D& operator*=(Vector2D& v, double value)
	{
		v.x_ *= value;
		v.y_ *= value;

		return v;
	}

	std::ostream& operator<<(std::ostream& out, const Vector2D& vec)
	{
		out << std::fixed << std::setprecision(1) << "[" << vec.x() << ", " << vec.y() << "]";
		return out;
	}

	std::istream& operator>>(std::istream& in, Vectors::Vector2D& vec)
	{
		// format: "[1.0, 2.0]"
		const char left_bracket = '[';
		const char right_bracket = ']';
		const char comma = ',';

		char start, separator, end;
		double x, y;

		if (in >> start && start != left_bracket)
		{
			in.unget();
			in.clear(std::ios_base::failbit);
			return in;
		}

		in >> x >> separator >> y >> end;


		if (!in || (separator != comma) || (end != right_bracket))
			throw std::runtime_error("Stream reading error");

		vec.x_ = x;
		vec.y_ = y;

		return in;
	}
}

using namespace Vectors;

TEST_CASE("vector2D")
{
	SECTION("default constructor")
	{
		Vectors::Vector2D vec;

		REQUIRE(vec.x() == Approx(0.0));
		REQUIRE(vec.y() == Approx(0.0));
	}

	SECTION("constructor with one argument")
	{
		Vectors::Vector2D vec(3.0);

		REQUIRE(vec.x() == 3.0);
		REQUIRE(vec.y() == 0.0);

		Vectors::Vector2D vec2(4.5);
	}

	SECTION("constructor")
	{
		const Vector2D vec(1.0, 2.0);

		REQUIRE(vec.x() == Approx(1.0));
		REQUIRE(vec.y() == Approx(2.0));
	}

	SECTION("length")
	{
		Vector2D vec(1.0, 1.0);

		REQUIRE(vec.length() == Approx(1.41).margin(0.01));
	}

	SECTION("unit vector x")
	{
		Vector2D vx = Vector2D::unit_x(); // static method

		REQUIRE(vx.x() == Approx(1.0));
		REQUIRE(vx.y() == Approx(0.0));
	}

	SECTION("unit vector y")
	{
		Vector2D vy = Vector2D::unit_y();

		REQUIRE(vy.x() == Approx(0.0));
		REQUIRE(vy.y() == Approx(1.0));
	}

	SECTION("operators")
	{

		const Vector2D vec1{ 1.0, 2.0 };
		const Vector2D vec2{ 2.0, 0.5 };

		SECTION("operator +")
		{
			auto vec = vec1 + vec2;

			REQUIRE(vec.x() == 3.0);
			REQUIRE(vec.y() == 2.5);
		}

		SECTION("equality")
		{
			REQUIRE(vec1 == Vector2D{ 1.0, 2.0 });
		}

		SECTION("nonequality")
		{
			REQUIRE(vec1 != vec2);
		}

		SECTION("subtraction")
		{
			auto vec = vec1 - vec2;

			REQUIRE(vec == Vector2D{ -1.0, 1.5 });
		}

		SECTION("negate")
		{
			auto vec = -vec1;

			REQUIRE(vec == Vector2D{ -1.0, -2.0 });
		}

		SECTION("multiplication")
		{
			double scalar = vec1 * vec2;

			REQUIRE(scalar == Approx(3.0));
		}

		SECTION("multiplication by double")
		{
			SECTION("a * Vector2D")
			{
				Vector2D vec = 2.0 * vec1;

				REQUIRE(vec == Vector2D{ 2.0, 4.0 });
			}

			SECTION("Vector2D * a")
			{
				Vector2D vec = vec1 * 2.0;

				REQUIRE(vec == Vector2D{ 2.0, 4.0 });
			}
		}

		SECTION("operator <<")
		{
			cout << "vec1: " << vec1 << endl;

			stringstream ss;

			ss << vec1;

			REQUIRE(ss.str() == "[1.0, 2.0]");
		}

		SECTION("operator >>")
		{
			string input("[1.0, 2.0]");

			stringstream ss(input);

			Vector2D vec;
			ss >> vec;

			REQUIRE(vec == Vector2D(1.0, 2.0));
		}
	}
}

TEST_CASE("operators")
{
	Vector2D v1(1.0, 2.0);
	Vector2D v2(3.0, 0.5);

	std::cout << -(v1 + v2 * (v2 * (-v1 * 4.0))) << "\n";

	auto result = v1 + v2;

	v1 *= 3.0;
	REQUIRE(v1 == Vector2D{ 3.0, 6.0 });
}
