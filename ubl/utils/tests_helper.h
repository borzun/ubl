#pragma once

#include "ubl.h"

#include <functional>
#include <vector>
#include <list>

UBL_NAMESPACE_BEGIN

namespace tests_helper
{
	std::vector<int> generateRandomVector(size_t size);
	std::list<int> generateRandomList(size_t size);
	std::vector<int> generateVeryLargeRandomVector();
	int createRandomValue(int START, int END);
	
	size_t getCurrentProcessRamMemoryUsage();

	template <class ExceptionType>
	bool wait_for_exception(std::function<void(void)> func_body)
	{
		try
		{
			func_body();
		}
		catch (const std::decay_t<ExceptionType>& exc)
		{
			return exc.what() != nullptr;
		}

		return false;
	}
}


UBL_NAMESPACE_END
