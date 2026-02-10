// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <isig/CPulseParamsComp.h>


// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


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
	static iser::CArchiveTag delayTimeTag("DelayTime", "Delay time for strobe pulses in seconds", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag pulseDurationTag("PulseDuration", "Duration of strobe pulses in seconds", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag relaxingTimeTag("RelaxingTime", "Time of relaxing after strobe pulse in seconds", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(delayTimeTag);
	retVal = retVal && archive.Process(m_delayTime);
	retVal = retVal && archive.EndTag(delayTimeTag);

	retVal = retVal && archive.BeginTag(pulseDurationTag);
	retVal = retVal && archive.Process(m_pulseDuration);
	retVal = retVal && archive.EndTag(pulseDurationTag);

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


