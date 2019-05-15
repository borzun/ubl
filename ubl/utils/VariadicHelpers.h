#pragma once

#include "ubl.h"
#include "detail/VariadicHelpersImpl.h"

UBL_NAMESPACE_BEGIN

// ---- MAX_SIZEOF function ----

// partial specialization
template <typename T = void>
constexpr size_t variadic_max_sizeof()
{
    return detail::MaxSizeof<T>::value;
}

// full specialization
template <typename T1, typename T2, typename ... Args>
constexpr size_t variadic_max_sizeof()
{
	// TODO: think about usage of MaxSizeof::value here - for consistenty purposes
	return std::max(variadic_max_sizeof<T1>(), variadic_max_sizeof < T2, Args...>());
}

// ---- IS_TYPE_VARIADIC ----
template <typename T = void>
constexpr bool is_type_variadic()
{
	return false;
}

template <typename OriginType, typename T1, typename ... Args>
constexpr bool is_type_variadic()
{
	return std::is_same_v<std::decay_t<OriginType>, std::decay_t<T1>> || is_type_variadic<OriginType, Args...>();
}

// ---- GET_TYPE_INDEX ----

template <typename OriginType = void>
constexpr size_t get_type_index()
{
	return ubl_npos;
}

template <typename OriginType, typename ... Args>
constexpr size_t get_type_index()
{
	return detail::get_type_index_impl<OriginType, Args...>(0);
}

UBL_NAMESPACE_END
