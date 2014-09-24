#include "iipr/CImageInterpolationParamsComp.h"


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CImageInterpolationParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CChangeNotifier notifier(this);

	SetInterpolationMode(*m_interpolationModeAttrPtr);
}


} // namespace iipr


