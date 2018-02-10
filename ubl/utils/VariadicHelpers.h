#pragma once

#include "ubl.h"
#include "detail\VariadicHelpersImpl.h"

UBL_NAMESPACE_BEGIN

// MAX_SIZEOF function
// full specialization
template <typename T1, typename T2, typename ... Args>
constexpr size_t variadic_max_sizeof()
{
	// TODO: think about usage of MaxSizeof::value here - for consistenty purposes
	return std::max(variadic_max_sizeof<T1>(), variadic_max_sizeof < T2, Args...>());
}

// partial specialization
template <typename T = void>
constexpr size_t variadic_max_sizeof()
{
	return typename detail::MaxSizeof<T>::value;
}

// IS_TYPE_VARIADIC
template <typename T = void>
constexpr bool is_type_variadic()
{
	return false;
}

template <typename OriginType, typename T1, typename ... Args>
constexpr bool is_type_variadic()
{
	// TODO: need to check - whether use std::is_same or std::is_convertible
	return std::is_same_v<OriginType, T1> || is_type_variadic<OriginType, Args...>();
}

UBL_NAMESPACE_END
