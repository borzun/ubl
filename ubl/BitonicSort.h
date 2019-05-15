#pragma once

#include "ubl.h"
#include <iterator>

UBL_NAMESPACE_BEGIN

template <class Iterator>
void bitonicSort(Iterator begin, Iterator end);


template <class Container>
void bitonicSort(Container& c)
{
	bitonicSort(std::begin(c), std::end(c));
}

void testAPI();

UBL_NAMESPACE_END

#include "detail/BitonicSortImpl.h"
