#pragma once

#include "ubl.h"

#include <detail\ParallelSearchAsync.h>

UBL_NAMESPACE_BEGIN

template <class IterType, class ValueType>
class ParallelSearch;


template <class IterType, class ValueType, class SearcherImpl = detail::ParallelSearchAsync<IterType, ValueType>>
bool parallelSearch(IterType begin, IterType end, const ValueType& value)
{
	const auto SIZE = end - begin;
	if (SIZE < 1000) {
		auto foundIter = std::find(begin, end, value);
		return foundIter != end;
	}

	SearcherImpl searcher(begin, end, value);
	return searcher();
}



UBL_NAMESPACE_END