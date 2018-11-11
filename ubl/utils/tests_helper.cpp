#include "tests_helper.h"

#include <random>
#include <algorithm>
#include <numeric>

#include "detail\IRamMonitor.h"

UBL_NAMESPACE_BEGIN

namespace tests_helper
{
	std::vector<int> generateRandomVector(size_t size)
	{
		std::vector<int> vec(size);
		
		std::iota(vec.begin(), vec.end(), 0);

		std::random_device device;
		std::mt19937 engine{ device() };

		std::shuffle(vec.begin(), vec.end(), engine);

		return vec;
	}

	std::list<int> generateRandomList(size_t size)
	{
		auto vec = generateRandomVector(size);
		return std::list<int>(vec.begin(), vec.end());
	}

	std::vector<int> generateVeryLargeRandomVector()
	{
		static const size_t large_size = static_cast<size_t>(std::pow(2, 15));

		std::vector<int> temp_vec;
		const size_t max_size = std::min(large_size, temp_vec.max_size() - 1);

		return generateRandomVector(max_size);
	}

	int createRandomValue(int START, int END)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		const std::uniform_int_distribution<std::mt19937::result_type> distributor(START, END); 

		return distributor(rng);
	}

	size_t getCurrentProcessRamMemoryUsage()
	{
		return detail::IRamMonitor::getInstance().getUsedMemoryByCurrentProcess();
	}
}

UBL_NAMESPACE_END