#include <icalib/CSimpleLensCorrectionComp.h>


namespace icalib
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CSimpleLensCorrectionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetDistortionFactor(*m_defaultDistortionFactorAttrPtr);
	SetOpticalCenter(i2d::CVector2d(*m_defaultOpticalCenterXAttrPtr, *m_defaultOpticalCenterYAttrPtr));
	SetDoDistortionOnly(*m_doDistortionOnlyAttrPtr);
}


} // namespace icalib


