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

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END