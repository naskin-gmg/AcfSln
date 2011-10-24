#include "isig/CTriggerParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace isig
{


CTriggerParamsComp::CTriggerParamsComp()
:	m_triggerMode(TM_NONE)
{
}


// reimplemented (isig::ITriggerParams)

const ITriggerConstraints* CTriggerParamsComp::GetTriggerConstraints() const
{
	return m_triggerConstraintsCompPtr.GetPtr();
}


int CTriggerParamsComp::GetTriggerMode() const
{
	return m_triggerMode;
}


void CTriggerParamsComp::SetTriggerMode(int triggerMode)
{
	if (m_triggerMode!= triggerMode){
		istd::CChangeNotifier changePtr(this);

		m_triggerMode = triggerMode;
	}
}


// reimplemented (iser::ISerializable)

bool CTriggerParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag bottomTag("TriggerMode", "Trigger mode");
	retVal = retVal && archive.BeginTag(bottomTag);
	retVal = retVal && archive.Process(m_triggerMode);
	retVal = retVal && archive.EndTag(bottomTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CTriggerParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_triggerMode = *m_triggerModeAttrPtr;
}


} // namespace isig


