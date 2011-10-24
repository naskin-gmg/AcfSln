#include "isig/CPulseParamsComp.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace isig
{


// public methods

CPulseParamsComp::CPulseParamsComp()
:	m_delayTime(1),
	m_pulseDuration(1),
	m_relaxingTime(1)
{
}


// reimplemented (isig::IPulseParams)

const IPulseConstraints* CPulseParamsComp::GetPulseConstraints() const
{
	return m_pulseConstraintsCompPtr.GetPtr();
}


double CPulseParamsComp::GetDelayTime() const
{
	return m_delayTime;
}


void CPulseParamsComp::SetDelayTime(double time)
{
	m_delayTime = time;
}


double CPulseParamsComp::GetPulseDuration() const
{
	return m_pulseDuration;
}


void CPulseParamsComp::SetPulseDuration(double time)
{
	m_pulseDuration = time;
}


double CPulseParamsComp::GetRelaxingTime() const
{
	return m_relaxingTime;
}


void CPulseParamsComp::SetRelaxingTime(double time)
{
	m_relaxingTime = time;
}


// reimplemented (iser::ISerializable)

bool CPulseParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag delayTimeTag("DelayTime", "Delay time for strobe pulses in seconds");
	retVal = retVal && archive.BeginTag(delayTimeTag);
	retVal = retVal && archive.Process(m_delayTime);
	retVal = retVal && archive.EndTag(delayTimeTag);

	static iser::CArchiveTag pulseDurationTag("PulseDuration", "Duration of strobe pulses in seconds");
	retVal = retVal && archive.BeginTag(pulseDurationTag);
	retVal = retVal && archive.Process(m_pulseDuration);
	retVal = retVal && archive.EndTag(pulseDurationTag);

	static iser::CArchiveTag relaxingTimeTag("RelaxingTime", "Time of relaxing after strobe pulse in seconds");
	retVal = retVal && archive.BeginTag(relaxingTimeTag);
	retVal = retVal && archive.Process(m_relaxingTime);
	retVal = retVal && archive.EndTag(relaxingTimeTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CPulseParamsComp::OnComponentCreated()
{
	m_delayTime = *m_delayTimeAttrPtr;
	m_pulseDuration = *m_pulseDurationAttrPtr;
	m_relaxingTime = *m_relaxingTimeAttrPtr;
}


} // namespace isig


