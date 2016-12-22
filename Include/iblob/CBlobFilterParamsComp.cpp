#include <iblob/CBlobFilterParamsComp.h>


namespace iblob
{


// reimplemented (icomp::CComponentBase)

void CBlobFilterParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetSupportedProperties(m_supportedPropertiesCompPtr.GetPtr());
}


} // namespace iblob


