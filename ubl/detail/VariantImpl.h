#pragma once

#include "ubl.h"
#include <type_traits>

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

template <size_t StorageLen, size_t StorageAlign>
class IVariantImpl
{
public:
	using StorageType = std::aligned_storage_t<StorageLen, StorageAlign>;

	IVariantImpl(StorageType& storage)
		: m_storage(storage)
	{ }

	virtual ~IVariantImpl() = default;

	virtual const std::type_info& getTypeInfo() const = 0;

protected:
	StorageType& m_storage;

private:
	IVariantImpl(const IVariantImpl& other) = delete;
	IVariantImpl& operator=(const IVariantImpl& other) = delete;
};


// Helper implementation for Variant object
template <class T, size_t StorageLen, size_t StorageAlign>
class VariantImpl
	: public IVariantImpl<StorageLen, StorageAlign>
{
public:
	using Type = T;
public:
	using SuperType = IVariantImpl<StorageLen, StorageAlign>;

	explicit VariantImpl(StorageType& storage)
		: SuperType(storage)
	{ }

	~VariantImpl()
	{
		if (const auto pValue = reinterpret_cast<T*>(&m_storage)) {
			pValue->~T();
		}
	}

	virtual const std::type_info& getTypeInfo() const
	{
		return typeid(T);
	}
};

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END
