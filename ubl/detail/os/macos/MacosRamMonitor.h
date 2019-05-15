#pragma once

#include "../../IRamMonitor.h"

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

class MacosRamMonitor
	: public IRamMonitor
{
public:

	MacosRamMonitor() = default;

	virtual size_t getUsedMemoryByCurrentProcess() const override;

};

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END
