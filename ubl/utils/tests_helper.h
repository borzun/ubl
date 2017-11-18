#pragma once

#include "ubl.h"

#include <functional>
#include <vector>

UBL_NAMESPACE_BEGIN

namespace tests_helper
{
	std::vector<int> generateRandomVector(size_t size);
	std::vector<int> generateVeryLargeRandomVector();
	
	size_t getCurrentProcessRamMemoryUsage();
}


UBL_NAMESPACE_END