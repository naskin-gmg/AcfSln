#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <imeas/TMeasurementValue.h>


namespace imeas
{


/**
	Extended message supporting attach of measurement information.
*/
template<class BaseMessageType, class ValueType = double>
class TMeasurementMessage: public BaseMessageType, virtual public iser::ISerializable
{
public:
	typedef BaseMessageType BaseClass;

	explicit TMeasurementMessage();
	TMeasurementMessage(istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL);

	void SetMeasurement(const TMeasurementValue<ValueType>& measurement);
	TMeasurementValue<ValueType> GetMeasurement() const;
	const TMeasurementValue<ValueType>& GetMeasurementRef() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (iser::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode = CM_STRICT) override;
	virtual istd::IChangeableUniquePtr CloneMe(istd::IChangeable::CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	TMeasurementValue<ValueType> m_measurementValue;
};


// public methods

template<class BaseMessageType, class ValueType>
TMeasurementMessage<BaseMessageType, ValueType>::TMeasurementMessage()
	:BaseClass()
{
}


template<class BaseMessageType, class ValueType>
TMeasurementMessage<BaseMessageType, ValueType>::TMeasurementMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& text,
			const QString& source,
			int flags,
			const QDateTime* timeStampPtr)
	:BaseClass(category, id, text, source, flags, timeStampPtr)
{
}


template<class BaseMessageType, class ValueType>
void TMeasurementMessage<BaseMessageType, ValueType>::SetMeasurement(const TMeasurementValue<ValueType>& measurement)
{
	m_measurementValue.SetErrorRange(measurement.GetErrorRange());
	m_measurementValue.SetWarningRange(measurement.GetWarningRange());
	m_measurementValue.SetMeasuredValue(measurement.GetMeasuredValue());
}


template<class BaseMessageType, class ValueType>
TMeasurementValue<ValueType> TMeasurementMessage<BaseMessageType, ValueType>::GetMeasurement() const
{
	return m_measurementValue;
}


template<class BaseMessageType, class ValueType>
const TMeasurementValue<ValueType>& TMeasurementMessage<BaseMessageType, ValueType>::GetMeasurementRef() const
{
	return *m_measurementValue;
}


// reimplemented (iser::ISerializable)
template<class BaseMessageType, class ValueType>
bool TMeasurementMessage<BaseMessageType, ValueType>::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && m_measurementValue.Serialize(archive);
	retVal = retVal && BaseClass::Serialize(archive);
 
	return retVal;
}


// reimplemented (iser::IChangeable)
template<class BaseMessageType, class ValueType>
bool TMeasurementMessage<BaseMessageType, ValueType>::CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	if (BaseClass::CopyFrom(object, mode)){

		const TMeasurementMessage* messageObjectPtr = dynamic_cast<const TMeasurementMessage*>(&object);
		if (messageObjectPtr != NULL){
			m_measurementValue.CopyFrom(messageObjectPtr->GetMeasurement(), mode);
		}

		return true;
	}

	return false;
}


template<class BaseMessageType, class ValueType>
istd::IChangeable* TMeasurementMessage<BaseMessageType, ValueType>::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr<TMeasurementMessage<BaseMessageType, ValueType>> clonedPtr(new TMeasurementMessage<BaseMessageType, ValueType>);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopPtr();
	}

	return NULL;
}


} // namespace imeas


