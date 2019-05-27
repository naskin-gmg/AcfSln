#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <istd/TRange.h>


namespace imeas
{


template<class ValueType = double>
class TMeasurementValue: virtual public iser::ISerializable
{
public:
	TMeasurementValue();
	TMeasurementValue(const ValueType& measuredValue, const istd::TRange<ValueType>& warningRange, const istd::TRange<ValueType>& errorRange);

	void SetErrorRange(const istd::TRange<ValueType>& errorRange);
	void SetWarningRange(const istd::TRange<ValueType>& warningRange);
	void SetMeasuredValue(const ValueType& measuredValue);

	istd::TRange<ValueType> GetErrorRange() const;
	istd::TRange<ValueType> GetWarningRange() const;
	const ValueType& GetMeasuredValue() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (iser::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode = CM_STRICT) override;
	virtual istd::IChangeable* CloneMe(istd::IChangeable::CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	static bool SerializeRange(iser::IArchive& archive, istd::TRange<ValueType> range, const iser::CArchiveTag& tag);

private:
	istd::TRange<ValueType> m_errorRange;
	istd::TRange<ValueType> m_warningRange;

	ValueType m_measuredValue;
};


// public methods

template<class ValueType>
TMeasurementValue<ValueType>::TMeasurementValue()
{
	m_errorRange.Reset();
	m_warningRange.Reset();
}


template<class ValueType>
TMeasurementValue<ValueType>::TMeasurementValue(
			const ValueType& measuredValue,
			const istd::TRange<ValueType>& warningRange,
			const istd::TRange<ValueType>& errorRange)
{
	m_errorRange.Reset();
	m_warningRange.Reset();
}


template<class ValueType>
void TMeasurementValue<ValueType>::SetErrorRange(const istd::TRange<ValueType>& errorRange)
{
	if (m_errorRange != errorRange){
		
		if (!(m_warningRange.IsEmpty())){
			if (errorRange.Contains(m_warningRange)){
				m_errorRange = errorRange;
			}
			else{ // clip to warning
			}
		}
		else{
			m_errorRange = errorRange;
		}
	}
}


template<class ValueType>
void TMeasurementValue<ValueType>::SetWarningRange(const istd::TRange<ValueType>& warningRange)
{
	if (m_warningRange != warningRange){

		if (!(m_errorRange.IsEmpty())){
			if (m_errorRange.Contains(warningRange)){
				m_warningRange = warningRange;
			}
			else{
				//message
			}
		}
		else{
			m_warningRange = warningRange;
		}
	}
}


template<class ValueType>
void TMeasurementValue<ValueType>::SetMeasuredValue(const ValueType& value)
{
	if (m_measuredValue != value){
		m_measuredValue = value;
	}
}


template<class ValueType>
istd::TRange<ValueType> TMeasurementValue<ValueType>::GetErrorRange() const
{
	return m_errorRange;
}


template<class ValueType>
istd::TRange<ValueType> TMeasurementValue<ValueType>::GetWarningRange() const
{
	return m_warningRange;
}


template<class ValueType>
const ValueType& TMeasurementValue<ValueType>::GetMeasuredValue() const
{
	return m_measuredValue;
}


// reimplemented (iser::ISerializable)
template<class ValueType>
bool TMeasurementValue<ValueType>::Serialize(iser::IArchive& archive)
{
	static const iser::CArchiveTag warningRangeTag("WarningRange", "Outliers of the range are warnings", iser::CArchiveTag::TT_LEAF);
	static const iser::CArchiveTag errorRangeTag("ErrorRange", "Outliers of the range are errors", iser::CArchiveTag::TT_LEAF);
	static const iser::CArchiveTag measuredValueTag("MeasuredValue", "Measured value", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(measuredValueTag);
	retVal = retVal && archive.Process(m_measuredValue);
	retVal = retVal && archive.EndTag(measuredValueTag);

	retVal = retVal && SerializeRange(archive, m_warningRange, warningRangeTag);
	retVal = retVal && SerializeRange(archive, m_errorRange, errorRangeTag);

	return retVal;
}


// reimplemented (iser::IChangeable)
template<class ValueType>
bool TMeasurementValue<ValueType>::CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode /*mode*/)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	const TMeasurementValue* messageObjectPtr = dynamic_cast<const TMeasurementValue*>(&object);
	if (messageObjectPtr != NULL){
		m_errorRange = messageObjectPtr->GetErrorRange();
		m_warningRange = messageObjectPtr->GetWarningRange();
		m_measuredValue = messageObjectPtr->GetMeasuredValue();

		return true;
	}

	return false;
}


template<class ValueType>
istd::IChangeable* TMeasurementValue<ValueType>::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr<TMeasurementValue<ValueType>> clonedPtr(new TMeasurementValue<ValueType>);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopPtr();
	}

	return NULL;
};


// private static methods

template<class ValueType>
bool TMeasurementValue<ValueType>::SerializeRange(iser::IArchive& archive, istd::TRange<ValueType> range, const iser::CArchiveTag& tag)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(tag);
	if (archive.IsStoring()){
		ValueType minValue = range.GetMinValue();
		ValueType maxValue = range.GetMaxValue();
		retVal = retVal && archive.Process(minValue);
		retVal = retVal && archive.Process(maxValue);
	}
	else{
		ValueType minValue = 0;
		ValueType maxValue = 0;
		retVal = retVal && archive.Process(minValue);
		retVal = retVal && archive.Process(maxValue);
		if (!retVal){
			return false;
		}

		range.SetMinValue(minValue);
		range.SetMaxValue(maxValue);
	}
	retVal = retVal && archive.EndTag(tag);

	return retVal;
}


} // namespace imeas


