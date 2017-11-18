#include <string>
#include <vector>

#include "utils\VariadicHelpers.h"

#include "gtest\gtest.h"

TEST(TestVariadicHelpers, TestVariadicMaxSizeof)
{
	// test 0 parameters
	ASSERT_EQ(0, ubl::variadic_max_sizeof<>());
	
	// test 1 parameter
	ASSERT_EQ(sizeof(int),				ubl::variadic_max_sizeof<int>());
	ASSERT_EQ(sizeof(double),			ubl::variadic_max_sizeof<double>());
	ASSERT_EQ(sizeof(std::vector<int>), ubl::variadic_max_sizeof<std::vector<int>>());

	// test 2 parameters
	// TODO: for some reason, there is a compile error when do not include call of variadic_max_sizeof into brackets...
	ASSERT_EQ(sizeof(double),	(ubl::variadic_max_sizeof<double, int>()));
	ASSERT_EQ(sizeof(short),	(ubl::variadic_max_sizeof<char, short>()));

	// test 3+ parameters
	ASSERT_EQ(sizeof(double),		(ubl::variadic_max_sizeof<bool, char, short, int, long, double>()));
	ASSERT_EQ(sizeof(double),		(ubl::variadic_max_sizeof<double, long, int, short, char, bool>()));
	ASSERT_EQ(sizeof(std::string),	(ubl::variadic_max_sizeof<int, char, std::string, bool, bool, bool>()));
}