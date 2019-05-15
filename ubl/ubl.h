#pragma once

#include <cstddef>

// ---- UBL namespace ----

#define UBL_NAMESPACE_BEGIN namespace ubl {
#define UBL_NAMESPACE_END }

UBL_NAMESPACE_BEGIN
	const size_t ubl_npos = -1;
UBL_NAMESPACE_END


//  Platform specific defines:
#ifdef _WIN32
    #define UBL_FORCEINLINE __forceinline
#else
    #define UBL_FORCEINLINE inline
#endif

// ---- detail namespace ----

#define DETAIL_NAMESPACE_BEGIN namespace detail {
#define DETAIL_NAMESPACE_END }

// ---- utils namesapce ----

#define UTILS_NAMESPACE_BEGIN namespace utils {
#define UTILS_NAMESPACE_END }
