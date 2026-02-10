// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icam/CExposureParamsComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TRange.h>
#include <iser/CArchiveTag.h>


namespace icam
{


CExposureParamsComp::CExposureParamsComp()
	:m_shutterTime(0.001), m_delayTime(0), m_eenDelayTime(0), m_gain(0)
{
}


// reimplemented (icam::IExposureParams)

const IExposureConstraints* CExposureParamsComp::GetExposureConstraints() const
{
	return m_exposureConstraintsCompPtr.GetPtr();
}


double CExposureParamsComp::GetShutterTime() const
{
	return m_shutterTime;
}


bool CExposureParamsComp::SetShutterTime(double time)
{
	if (m_exposureConstraintsCompPtr.IsValid()){
		istd::CRange range = m_exposureConstraintsCompPtr->GetShutterTimeRange(GetCurrentCameraIndex());

		if (!range.Contains(time)){
			return false;
		}
	}

	if (time != m_shutterTime){
		istd::CChangeNotifier notifier(this);

		m_shutterTime = time;
	}

	return true;
}


double CExposureParamsComp::GetDelayTime() const
{
	return m_delayTime;
}


bool CExposureParamsComp::SetDelayTime(double time)
{
	if (m_exposureConstraintsCompPtr.IsValid()){
		istd::CRange range = m_exposureConstraintsCompPtr->GetDelayTimeRange(GetCurrentCameraIndex());

		if (!range.Contains(time)){
			return false;
		}
	}

	if (time != m_delayTime){
		istd::CChangeNotifier notifier(this);

		m_delayTime = time;
	}
	
	return true;
}


double CExposureParamsComp::GetEenDelay() const
{
	return m_eenDelayTime;
}


bool CExposureParamsComp::SetEenDelay(double time)
{
	if (m_exposureConstraintsCompPtr.IsValid()){
		istd::CRange range = m_exposureConstraintsCompPtr->GetEenDelayRange(GetCurrentCameraIndex());
		if (!range.Contains(time)){
			return false;
		}
	}

	if (time != m_eenDelayTime){
		istd::CChangeNotifier notifier(this);

		m_eenDelayTime = time;
	}

	return true;
}


int CExposureParamsComp::GetGainValue() const
{
	return m_gain;
}


bool CExposureParamsComp::SetGainValue(int gain)
{
	if (m_exposureConstraintsCompPtr.IsValid()) {
		istd::CIntRange range = m_exposureConstraintsCompPtr->GetGainRange(GetCurrentCameraIndex());
		if (!range.Contains(gain)) {
			return false;
		}
	}

	if (gain!= m_gain) {
		istd::CChangeNotifier notifier(this);

		m_gain = gain;
	}

	return true;
}

int CExposureParamsComp::GetCurrentCameraIndex() const
{
	return m_cameraSelectionCompPtr.IsValid() ? m_cameraSelectionCompPtr->GetSelectedOptionIndex() : 0;
}


// reimplemented (iser::ISerializable)

bool CExposureParamsComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag shutterTimeTag("ShutterTime", "Shutter time in seconds", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag delayTimeTag("DelayTime", "Delay of shutter in seconds", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag eenDelayTimeTag("EenDelayTime", "Delay of exposure enabled signal in seconds", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag gainTag("Gain", "Gain in dB", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(shutterTimeTag);
	retVal = retVal && archive.Process(m_shutterTime);
	retVal = retVal && archive.EndTag(shutterTimeTag);

	retVal = retVal && archive.BeginTag(delayTimeTag);
	retVal = retVal && archive.Process(m_delayTime);
	retVal = retVal && archive.EndTag(delayTimeTag);

	retVal = retVal && archive.BeginTag(eenDelayTimeTag);
	retVal = retVal && archive.Process(m_eenDelayTime);
	retVal = retVal && archive.EndTag(eenDelayTimeTag);

	retVal = retVal && archive.BeginTag(gainTag);
	retVal = retVal && archive.Process(m_gain);
	retVal = retVal && archive.EndTag(gainTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CExposureParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_defaultShutterTimeAttrPtr.IsValid());	// isObligatory is set
	m_shutterTime = *m_defaultShutterTimeAttrPtr;

	Q_ASSERT(m_defaultGainAttrPtr.IsValid());	// isObligatory is set
	m_gain = *m_defaultGainAttrPtr;

	if (m_cameraSelectionModelCompPtr.IsValid()) {
		m_cameraSelectionModelCompPtr->AttachObserver(this);
	}
}

void CExposureParamsComp::OnComponentDestroyed()
{
	if (m_cameraSelectionModelCompPtr.IsValid()) {
		m_cameraSelectionModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}

void CExposureParamsComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	istd::CChangeNotifier notifier(this);
}


} // namespace icam


