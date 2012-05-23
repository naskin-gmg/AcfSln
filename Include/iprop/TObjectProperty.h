#ifndef iprop_TObjectProperty_included
#define iprop_TObjectProperty_included


#include "istd/CClassInfo.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iprop/CPropertyBase.h"


namespace iprop
{


/**
	Template implementation of an atomic property.
*/
template <typename Value>
class TObjectProperty: public CPropertyBase
{
public:
	typedef Value ValueType;
	typedef TObjectProperty<Value> PropertyType;
	typedef CPropertyBase BaseClass;

	TObjectProperty();

	TObjectProperty(
				IPropertiesManager* propertyOwnerPtr,
				const QByteArray& propertyName,
				const QByteArray& propertyDescription,
				int propertyFlags,
				int changeFlags = 0,
				const ValueType& defaultValue = ValueType());

	const Value& GetValue() const;
	bool SetValue(const Value& value);
	void ResetValue();

	// reimplemented (iser::IProperty)
	virtual const iser::IObject* GetDefaultPropertyPtr() const;

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static const QByteArray& GetTypeName();

protected:
	iser::TCopySerializedWrap<Value> m_value;

	mutable istd::TDelPtr<PropertyType> m_defaultPropertyValuePtr;

private:
	static const QByteArray s_typeName;
};


// public methods

template <typename Value>
TObjectProperty<Value>::TObjectProperty()
	:BaseClass()
{
}


template <typename Value>
TObjectProperty<Value>::TObjectProperty(
				IPropertiesManager* propertyOwnerPtr,
				const QByteArray& propertyName,
				const QByteArray& propertyDescription,
				int propertyFlags,
				int changeFlags,
				const ValueType& defaultValue)
	:BaseClass(propertyOwnerPtr, propertyName, propertyDescription, propertyFlags, changeFlags)
{
	m_defaultPropertyValuePtr.SetPtr(new PropertyType());

	m_defaultPropertyValuePtr->SetValue(defaultValue);

	SetValue(defaultValue);
}


template <typename Value>
const Value& TObjectProperty<Value>::GetValue() const
{
	return m_value;
}


template <typename Value>
bool TObjectProperty<Value>::SetValue(const Value& value)
{
	if (m_value != value){
		return m_value.CopyFrom(value);
	}

	return true;
}


template <typename Value>
void  TObjectProperty<Value>::ResetValue()
{
	if (m_defaultPropertyValuePtr.IsValid()){
		SetValue(m_defaultPropertyValuePtr->GetValue());
	}
}


// reimplemented (iser::IProperty)

template <typename Value>
const iser::IObject* TObjectProperty<Value>::GetDefaultPropertyPtr() const
{
	return m_defaultPropertyValuePtr.GetPtr();
}


// reimplemented (iser::IObject)

template <typename Value>
QByteArray TObjectProperty<Value>::GetFactoryId() const
{
	return s_typeName;
}


// reimplemented (iser::ISerializable)

template <typename Value>
bool TObjectProperty<Value>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valueTag("Value", "Property value");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(valueTag);
	retVal = retVal && m_value.Serialize(archive);
	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


// static methods

template <typename Value>
const QByteArray& TObjectProperty<Value>::GetTypeName()
{
	return s_typeName;
}


// private static properties

template <typename Value>
const QByteArray TObjectProperty<Value>::s_typeName(istd::CClassInfo::GetName<TObjectProperty<Value> >());


} // namespace iprop


#endif // !iprop_TObjectProperty_included


