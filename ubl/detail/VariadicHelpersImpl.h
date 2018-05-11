#pragma once

#include "ubl.h"

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

template <typename T>
struct MaxSizeof
{
	static constexpr size_t value = sizeof(T);
};

template <>
struct MaxSizeof<void>
{
	static constexpr size_t value = 0;
};

// GET_TYPE_INDEX IMPL
template <typename T = void>
constexpr size_t get_type_index_impl(const size_t index)
{
	return ubl_npos;
}

template <typename OriginType, typename T1, typename ... Args>
constexpr size_t get_type_index_impl(const size_t index)
{
	if (std::is_same_v<std::decay_t<OriginType>, std::decay_t<T1>>) {
		return index;
	}

	return get_type_index_impl<OriginType, Args...>(index + 1);
}


DETAIL_NAMESPACE_END

UBL_NAMESPACE_END