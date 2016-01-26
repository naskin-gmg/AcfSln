#include "ifileproc/CFileNamingParamsComp.h"


namespace ifileproc
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileNamingParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_prefix = *m_prefixAttrPtr;
	m_suffix = *m_suffixAttrPtr;
	m_overwriteStrategy = OverwriteStrategy(*m_overwriteStrategyAttrPtr);
}


} // namespace ifileproc


