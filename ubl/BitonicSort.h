#pragma once

#include "ubl.h"

UBL_NAMESPACE_BEGIN

template <class Container>
void bitonicSort(Container& c)
{
	bitonicSort(std::begin(c), std::end(c));
}


template <class Iterator>
void bitonicSort(Iterator begin, Iterator end);


void testAPI();

UBL_NAMESPACE_END

#include "detail\BitonicSortImpl.h"
