#include <iipr/CCaliperParamsComp.h>


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CCaliperParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CChangeNotifier notifier(this);

	SetWeightThreshold(*m_weightThresholdAttrPtr);
	SetPolarityMode(*m_polarityModeAttrPtr);
	SetDirectionMode(*m_directionModeAttrPtr);
}


} // namespace iipr


