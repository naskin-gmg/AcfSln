#ifndef iprop_TMultiProperty_included
#define iprop_TMultiProperty_included


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QStringList>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "iser/IArchive.h"
#include "iser/IObject.h"
#include "iser/CArchiveTag.h"

// ACF-Solutions includes
#include "iprop/CPropertyBase.h"


namespace iprop
{


/**
	Template implementation of multiple property.
	\internal
*/
template <typename Value, class Container = QVector<Value> >
class TMultiProperty: public CPropertyBase
{
public:
	typedef Value ValueType;
	typedef CPropertyBase BaseClass;
	typedef Container ValueList;
	typedef typename ValueList::iterator iterator;
	typedef typename ValueList::const_iterator const_iterator;

	/**
		Default constructor.
	*/
	TMultiProperty();

	/**
		Constructor.
		\param	propertyOwnerPtr	Owner of the attrbiute.
		\param	propertyId		Unique ID of the attrbiute.
		\param	propertyId		Property description.
		\param	changeFlags		Change flags for model update notification.
		\param	elementsCount	Number of elements stored at \c valuesPtr.
		\param	valuesPtr		Pointer to array of elements.
	*/
	TMultiProperty(
				IPropertiesManager* propertyOwnerPtr,
				const QByteArray& propertyId,
				const QByteArray& propertyDescription,
				int propertyFlags,
				int changeFlags = 0,
				int elementsCount = 0,
				Value* valuesPtr = NULL);

	void SetValues(const ValueList& valueList);

	/**
		Set value list to the property.
	*/
	template <class ContainerImpl>
	void SetValues(typename ContainerImpl::const_iterator begin, typename ContainerImpl::const_iterator end);

	/**
		Get all values.
	*/
	const ValueList& GetValues() const;

	/**
		Get number of values in the multi property.
	*/
	virtual int GetValuesCount() const;

	/**
		Get value at the index \c index
	*/
	virtual Value GetValueAt(int index) const;

	/**
		Set value at the index \c index to new value \c value.
	*/
	virtual void SetValueAt(int index, const Value& value);

	/**
		Insert new value into container.
	*/
	virtual void InsertValue(const Value& value);

	/**
		Remove all values from the container.
	*/
	virtual void ResetValues();

	// STL compatibility
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	// reimplemented (iser::IObject)
	QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static const QByteArray& GetTypeName();

protected:
	Container m_values;

private:
	static const QByteArray s_typeName;
};


// public methods

template <typename Value, class Container>
TMultiProperty<Value, Container>::TMultiProperty()
{
}


template <typename Value, class Container>
TMultiProperty<Value, Container>::TMultiProperty(
				IPropertiesManager* propertyOwnerPtr,
				const QByteArray& propertyId,
				const QByteArray& propertyDescription,
				int propertyFlags,
				int changeFlags,
				int elementsCount,
				Value* valuesPtr)
	:BaseClass(propertyOwnerPtr, propertyId, propertyDescription, propertyFlags, changeFlags)
{
	for (int i = 0; i < elementsCount; ++i){
		m_values.push_back(valuesPtr[i]);
	}
}


template <typename Value, class Container>
void TMultiProperty<Value, Container>::SetValues(const ValueList& valueList)
{
	SetValues<ValueList>(valueList.begin(), valueList.end());
}


template <typename Value, class Container>
template <class ContainerImpl>
void TMultiProperty<Value, Container>::SetValues(typename ContainerImpl::const_iterator beginIter, typename ContainerImpl::const_iterator endIter)
{
	istd::CChangeNotifier changePtr(m_propertyOwnerPtr, m_changeFlags);

	ResetValues();

	while (beginIter != endIter){
		InsertValue(*beginIter);

		++beginIter;
	}
}


template <typename Value, class Container>
const typename TMultiProperty<Value, Container>::ValueList& TMultiProperty<Value, Container>::GetValues() const
{
	return m_values;
}


template <typename Value, class Container>
int TMultiProperty<Value, Container>::GetValuesCount() const
{
	return int(m_values.size());
}


template <typename Value, class Container>
Value TMultiProperty<Value, Container>::GetValueAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetValuesCount());

	return m_values[index];
}


template <typename Value, class Container>
void TMultiProperty<Value, Container>::SetValueAt(int index, const Value& value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetValuesCount());

	if (m_values[index] != value){
		istd::CChangeNotifier changePtr(m_propertyOwnerPtr, m_changeFlags);

		m_values[index] = value;
	}
}


template <typename Value, class Container>
void TMultiProperty<Value, Container>::InsertValue(const Value& value)
{
	m_values.push_back(value);
}


template <typename Value, class Container>
void TMultiProperty<Value, Container>::ResetValues()
{
	istd::CChangeNotifier changePtr(m_propertyOwnerPtr, m_changeFlags);
	
	m_values.clear();
}


// STL compatibility

template <typename Value, class Container>
typename TMultiProperty<Value, Container>::iterator TMultiProperty<Value, Container>::begin()
{
	return m_values.begin();
}


template <typename Value, class Container>
typename TMultiProperty<Value, Container>::const_iterator TMultiProperty<Value, Container>::begin() const
{
	return m_values.begin();
}


template <typename Value, class Container>
typename TMultiProperty<Value, Container>::iterator TMultiProperty<Value, Container>::end()
{
	return m_values.end();
}
	

template <typename Value, class Container>
typename TMultiProperty<Value, Container>::const_iterator TMultiProperty<Value, Container>::end() const
{
	return m_values.end();
}


// reimplemented (iser::IObject)

template <typename Value, class Container>
QByteArray TMultiProperty<Value, Container>::GetFactoryId() const
{
	return s_typeName;
}


// reimplemented (iser::ISerializable)

template <typename Value, class Container>
bool TMultiProperty<Value, Container>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valuesTag("Values", "List of property values");
	static iser::CArchiveTag valueTag("Value", "Single Value");

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool retVal = true;

	if (isStoring){
		int valuesCount = int(m_values.size());

		retVal = retVal && archive.BeginMultiTag(valuesTag, valueTag, valuesCount);
		for (ValueList::iterator iter = m_values.begin(); iter != m_values.end(); ++iter){
			retVal = retVal && archive.BeginTag(valueTag);
			retVal = retVal && archive.Process(*iter);
			retVal = retVal && archive.EndTag(valueTag);
		}
		retVal = retVal && archive.EndTag(valuesTag);
	}
	else{
		int valuesCount = 0;

		retVal = retVal && archive.BeginMultiTag(valuesTag, valueTag, valuesCount);
		if (!retVal){
			return false;
		}

		// load properties 'in place', without allocation of new elements
		int itemIndex = 0;
		for (ValueList::iterator iter = m_values.begin(); iter != m_values.end(); ++iter){
			// remove rest of elements, if current number of elements is bigger than the stored
			if (itemIndex >= valuesCount){
				m_values.erase(iter, m_values.end());

				break;
			}

			retVal = retVal && archive.BeginTag(valueTag);
			retVal = retVal && archive.Process(*iter);
			retVal = retVal && archive.EndTag(valueTag);

			++itemIndex;
		}

		// load rest of properties if current number of elements is smaller than the stored
		while (itemIndex < valuesCount){
			retVal = retVal && archive.BeginTag(valueTag);
			Value value;
			retVal = retVal && archive.Process(value);
			m_values.push_back(value);
			retVal = retVal && archive.EndTag(valueTag);

			++itemIndex;
		}

		retVal = retVal && archive.EndTag(valuesTag);
	}

	return retVal;
}


// static methods

template <typename Value, class Container>
const QByteArray& TMultiProperty<Value, Container>::GetTypeName()
{
	return s_typeName;
}


// private static properties

template <typename Value, class Container>
const QByteArray TMultiProperty<Value, Container>::s_typeName(istd::CClassInfo::GetName<TMultiProperty<Value> >());


// typedefs

typedef TMultiProperty<double> CMultiDoubleProperty;
typedef TMultiProperty<bool> CMultiBoolProperty;
typedef TMultiProperty<int> CMultiIntProperty;
typedef TMultiProperty<QString, QStringList> CMultiStringProperty;
typedef TMultiProperty<QByteArray> CMultiStdStringProperty;


} // namespace iprop


#endif // !iprop_TMultiProperty_included


