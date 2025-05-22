#include <iblob/CBlobFilterParamsComp.h>


namespace iblob
{


// public methods

// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CBlobFilterParamsComp::GetCalibration() const
{
	return m_calibrationCompPtr.GetPtr();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CBlobFilterParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetSupportedProperties(m_supportedPropertiesCompPtr.GetPtr());
	
	if (m_defaultAvailablePropertiesAttrPtr.IsValid() && m_defaultAvailablePropertiesAttrPtr.GetCount() > 0){
		const int count = m_defaultAvailablePropertiesAttrPtr.GetCount();
		for (int i = 0; i < count; ++i){
			const QByteArray filterId = m_defaultAvailablePropertiesAttrPtr[i];
			iblob::IBlobFilterParams::Filter oneFilter;
			oneFilter.featureId = filterId;
			oneFilter.operation = iblob::IBlobFilterParams::FilterOperation::FO_INCLUDE;
			oneFilter.condition = iblob::IBlobFilterParams::FC_GREATER;
			AddFilter(oneFilter);
		}
	}
}


} // namespace iblob


