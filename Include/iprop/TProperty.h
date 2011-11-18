#ifndef iprop_TProperty_included
#define iprop_TProperty_included


#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/TCopySerializedWrap.h"

#include "iprop/CPropertyBase.h"


namespace iprop
{


/**
	Template implementation of an atomic property.
*/
template <typename Value>
class TProperty: public CPropertyBase
{
public:
	typedef Value ValueType;
	typedef TProperty<Value> PropertyType;
	typedef CPropertyBase BaseClass;

	TProperty();

	TProperty(
				IPropertiesManager* propertyOwnerPtr,
				const std::string& propertyName,
				const std::string& propertyDescription,
				int propertyFlags,
				int changeFlags = 0,
				const ValueType& defaultValue = ValueType());

	const Value& GetValue() const;
	void SetValue(const Value& value);
	void ResetValue();

	// reimplemented (iser::IProperty)
	virtual const iser::IObject* GetDefaultPropertyPtr() const;

	// reimplemented (iser::IObject)
	virtual std::string GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static const std::string& GetTypeName();

protected:
	Value m_value;

	mutable istd::TDelPtr<PropertyType> m_defaultPropertyValuePtr;

private:
	static const std::string s_typeName;
};


// public methods

template <typename Value>
TProperty<Value>::TProperty()
	:BaseClass()
{
}


template <typename Value>
TProperty<Value>::TProperty(
				IPropertiesManager* propertyOwnerPtr,
				const std::string& propertyName,
				const std::string& propertyDescription,
				int propertyFlags,
				int changeFlags,
				const ValueType& defaultValue)
	:BaseClass(propertyOwnerPtr, propertyName, propertyDescription, propertyFlags, changeFlags),
	m_value(defaultValue)
{
	if (!m_defaultPropertyValuePtr.IsValid()){
		m_defaultPropertyValuePtr.SetPtr(new PropertyType());

		m_defaultPropertyValuePtr->SetValue(defaultValue);
	}
}


template <typename Value>
const Value& TProperty<Value>::GetValue() const
{
	return m_value;
}


template <typename Value>
void TProperty<Value>::SetValue(const Value& value)
{
	if (m_value != value){
		istd::CChangeNotifier changePtr(m_propertyOwnerPtr, m_changeFlags);

		m_value = value;
	}
}


template <typename Value>
void  TProperty<Value>::ResetValue()
{
	if (m_defaultPropertyValuePtr.IsValid()){
		SetValue(m_defaultPropertyValuePtr->GetValue());
	}
}


// reimplemented (iser::IProperty)

template <typename Value>
const iser::IObject* TProperty<Value>::GetDefaultPropertyPtr() const
{
	return m_defaultPropertyValuePtr.GetPtr();
}


// reimplemented (iser::IObject)

template <typename Value>
std::string TProperty<Value>::GetFactoryId() const
{
	return s_typeName;
}


// reimplemented (iser::ISerializable)

template <typename Value>
bool TProperty<Value>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag valueTag("Value", "Property value");
	retVal = retVal && archive.BeginTag(valueTag);

	retVal = retVal && archive.Process(m_value);

	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


// static methods

template <typename Value>
const std::string& TProperty<Value>::GetTypeName()
{
	return s_typeName;
}


// private static properties

template <typename Value>
const std::string TProperty<Value>::s_typeName(istd::CClassInfo::GetName<TProperty<Value> >());


// typedefs

typedef TProperty<double> CDoubleProperty;
typedef TProperty<bool> CBoolProperty;
typedef TProperty<int> CIntProperty;
typedef TProperty<istd::CString> CStringProperty;
typedef TProperty<std::string> CStdStringProperty;


} // namespace iprop


#endif // !iprop_TProperty_included


