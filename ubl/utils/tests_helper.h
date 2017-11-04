#pragma once

#include "ubl.h"
#include <vector>

UBL_NAMESPACE_BEGIN

namespace tests_helper
{
	std::vector<int> generateRandomVector(size_t size);
	std::vector<int> generateVeryLargeRandomVector();
}


UBL_NAMESPACE_END