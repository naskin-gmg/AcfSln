#include "iipr/CRectDerivativeProcessorComp.h"


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CRectDerivativeProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_filterParamsIdAttrPtr.IsValid());	// isObligatory was set to 'true'
	SetFilterParamsId((*m_filterParamsIdAttrPtr).ToString());
}


} // namespace iipr

