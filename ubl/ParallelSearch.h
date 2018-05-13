#pragma once

#include <stdexcept>

#include "ubl.h"

#include <detail\ParallelSearchAsync.h>
#include <detail\ParallelSearchOpenMP.h>

UBL_NAMESPACE_BEGIN

enum class ParallelSearchImplementation
{
	Async,
	OpenMP
};

// Parallel search with out-of the box implementations
template <class IterType, class ValueType>
bool parallelSearch(IterType begin, IterType end, const ValueType& value, ParallelSearchImplementation impl = ParallelSearchImplementation::Async)
{
	switch (impl) {
		case ParallelSearchImplementation::Async:
			return parallelSearch<IterType, ValueType, detail::ParallelSearchAsync<IterType, ValueType>>(begin, end, value);
		case ParallelSearchImplementation::OpenMP:
			return parallelSearch<IterType, ValueType, detail::ParallelSearchOpenMP<IterType, ValueType>>(begin, end, value);
		default:
			throw std::invalid_argument("Unsupported implementation type!");
	}
}

// Generic, extendible implementation of ParallelSearch
template <class IterType, class ValueType, class SearcherImpl>
bool parallelSearch(IterType begin, IterType end, const ValueType& value)
{
	const auto SIZE = std::distance(begin, end);
	if (SIZE < 1000) {
		auto foundIter = std::find(begin, end, value);
		return foundIter != end;
	}

	SearcherImpl searcher(begin, end, value);
	return searcher();
}


UBL_NAMESPACE_END