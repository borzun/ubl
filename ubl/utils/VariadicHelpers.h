#pragma once

#include "ubl.h"
#include "detail\VariadicHelpersImpl.h"

UBL_NAMESPACE_BEGIN

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

UBL_NAMESPACE_END
