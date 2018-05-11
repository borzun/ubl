#pragma once

#include "ubl.h"
#include "utils\VariadicHelpers.h"
#include "detail\VariantImpl.h"

#include <memory>
#include <type_traits>

UBL_NAMESPACE_BEGIN

// Exception, which is used in case of error while calling ::getValue()
class InvalidVariantException
	: public std::exception
{
public:
	explicit InvalidVariantException(std::string&& errorStr)
		: std::exception()
		, m_errorMsg(std::forward<std::string>(errorStr))
	{

	}

	virtual char const* what() const override
	{
		return m_errorMsg.c_str();
	}

	private:
		std::string m_errorMsg;

};

// Invalid index of Variant::getIndex() method
static constexpr size_t variant_npos = ubl_npos;

template <typename... Args>
class Variant
{
public:

	Variant() = default;

	// Assign some value of specified type to Variant
	template <typename T>
	Variant<Args...>& operator=(T&& value)
	{
		using Decayed_T = std::decay_t<T>;

		// In case user assigns the value of the type, which Variant already holds:
		if (m_impl && m_impl->getTypeInfo() == typeid(Decayed_T)) {
			if (Decayed_T* pValue = reinterpret_cast<Decayed_T*>(&m_storage)) {
				*pValue = std::forward<T>(value);
			}
			else {
				throw InvalidVariantException(std::string("Internal error - the Variant's storage of type: [") + m_impl->getTypeInfo().name() + "] is damaged!");
			}
		}
		else {
			if (is_type_variadic<T, Args...>()) {
				m_impl.reset(new detail::VariantImpl<Decayed_T, StorageLen, StorageAlign>(m_storage));
				new (&m_storage) Decayed_T(std::forward<T>(value));
			} else{
				throw InvalidVariantException(std::string("Type: " + std::string(typeid(T).name()) + " doesn't belong to variadic types!"));
			}
		}
		
		return *this;
	}

	// Return currently stored value as the requested type:
	template <typename T>
	T& getValue()
	{
		if (!m_impl) {
			throw InvalidVariantException(std::string("Can't get Variant's value of type: [") + typeid(T).name() + "] - Variant object is empty!");
		}

		using Decayed_T = std::decay_t<T>;
		if (typeid(Decayed_T) != m_impl->getTypeInfo()) {
			throw InvalidVariantException(std::string("Can't get Variant's value of type: [") + typeid(T).name() + "] - actual type is: [" + m_impl->getTypeInfo().name() + "]");
		}

		auto p_value = reinterpret_cast<Decayed_T*>(&m_storage);
		if (!p_value) {
			throw InvalidVariantException(std::string("Internal error - can't get variant's value of type: [") + typeid(T).name() + "]");
		}

		return *p_value;
	}

	template <typename T>
	const T& getValue() const
	{
		return const_cast<Variant<...Args>*>(this)->getValue<T>();
	}

	// ---- Observers ----
	size_t getIndex() const
	{
		if (!m_impl) {
			return variant_npos;
		}

		return 0;
	}

private:
	static constexpr size_t StorageLen = variadic_max_sizeof<Args...>();
	// TODO: need to provide some another alignment specifier (like max alignment of all parameter pack types)
	static constexpr size_t StorageAlign = std::alignment_of_v<double>;
	std::unique_ptr<detail::IVariantImpl<StorageLen, StorageAlign>> m_impl;
	
	using StorageType = std::aligned_storage_t<StorageLen, StorageAlign>;
	StorageType m_storage;
	
};

UBL_NAMESPACE_END
