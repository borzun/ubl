#include "tests_helper.h"

#include <algorithm>
#include <numeric>

UBL_NAMESPACE_BEGIN

namespace tests_helper
{
	std::vector<int> generateRandomVector(size_t size)
	{
		std::vector<int> vec(size);
		
		std::iota(vec.begin(), vec.end(), 0);
		std::random_shuffle(vec.begin(), vec.end());

		return vec;
	}
	std::vector<int> generateVeryLargeRandomVector()
	{
		static const size_t large_size = static_cast<size_t>(std::pow(2, 15));

		std::vector<int> temp_vec;
		const size_t max_size = std::min(large_size, temp_vec.max_size() - 1);

		return generateRandomVector(max_size);
	}
}

UBL_NAMESPACE_END