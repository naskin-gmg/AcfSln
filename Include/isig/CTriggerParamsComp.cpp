// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <isig/CTriggerParamsComp.h>


#include <istd/CChangeNotifier.h>

#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace isig
{


CTriggerParamsComp::CTriggerParamsComp()
:	m_triggerMode(TM_NONE),
	m_triggersCount(-1)
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


int CTriggerParamsComp::GetTriggersCount() const
{
	return m_triggersCount;
}


void CTriggerParamsComp::SetTriggersCount(int triggersCount)
{
	if (m_triggersCount != triggersCount){
		istd::CChangeNotifier changePtr(this);

		m_triggersCount = triggersCount;
	}
}



// reimplemented (iprm::ISelectionParam)

const iprm::IOptionsList* CTriggerParamsComp::GetSelectionConstraints() const
{
	return this;
}


int CTriggerParamsComp::GetSelectedOptionIndex() const
{
	Q_ASSERT(m_triggerMode >= 0);

	int optionsCount = int(m_selectionList.size());
	for (int i = 0; i < optionsCount; ++i){
		const SelectionEntry& entry = m_selectionList[i];
		if (entry.triggerMode == m_triggerMode){
			return i;
		}
	}

	return -1;
}


bool CTriggerParamsComp::SetSelectedOptionIndex(int index)
{
	if (index >= 0){
		SetTriggerMode(m_selectionList[index].triggerMode);

		return true;
	}
	else{
		return false;
	}
}


iprm::ISelectionParam* CTriggerParamsComp::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CTriggerParamsComp::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	bool retVal = true;

	static iser::CArchiveTag triggerModeTag("TriggerMode", "Trigger mode", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(triggerModeTag);
	retVal = retVal && archive.Process(m_triggerMode);
	retVal = retVal && archive.EndTag(triggerModeTag);

	static iser::CArchiveTag triggersCountTag("TriggersCount", "Number of triggers in the continuos mode", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(triggersCountTag);
	retVal = retVal && archive.Process(m_triggersCount);
	retVal = retVal && archive.EndTag(triggersCountTag);

	return retVal;
}


// protected methods

void CTriggerParamsComp::InitializeSelectionList()
{
	m_selectionList.clear();
	if (!m_triggerConstraintsCompPtr.IsValid() || m_triggerConstraintsCompPtr->IsTriggerModeSupported(TM_NONE)){
		SelectionEntry entry = {"None", "No trigger input", TM_NONE};
		m_selectionList.push_back(entry);
	}
	if (!m_triggerConstraintsCompPtr.IsValid() || m_triggerConstraintsCompPtr->IsTriggerModeSupported(TM_CONTINUOUS)){
		SelectionEntry entry = {"Continuous", "Continuous mode", TM_CONTINUOUS};
		m_selectionList.push_back(entry);
	}
	if (!m_triggerConstraintsCompPtr.IsValid() || m_triggerConstraintsCompPtr->IsTriggerModeSupported(TM_RISING_EDGE)){
		SelectionEntry entry = {"Rising edge", "Rising trigger edge", TM_RISING_EDGE};
		m_selectionList.push_back(entry);
	}
	if (!m_triggerConstraintsCompPtr.IsValid() || m_triggerConstraintsCompPtr->IsTriggerModeSupported(TM_FALLING_EDGE)){
		SelectionEntry entry = {"Falling edge", "Falling trigger edge", TM_FALLING_EDGE};
		m_selectionList.push_back(entry);
	}
	if (!m_triggerConstraintsCompPtr.IsValid() || m_triggerConstraintsCompPtr->IsTriggerModeSupported(TM_POSITIVE_LEVEL)){
		SelectionEntry entry = {"Positive level", "Positive level", TM_POSITIVE_LEVEL};
		m_selectionList.push_back(entry);
	}
	if (!m_triggerConstraintsCompPtr.IsValid() || m_triggerConstraintsCompPtr->IsTriggerModeSupported(TM_NEGATIVE_LEVEL)){
		SelectionEntry entry = {"Negative level", "Negative level", TM_NEGATIVE_LEVEL};
		m_selectionList.push_back(entry);
	}
	if (!m_triggerConstraintsCompPtr.IsValid() || m_triggerConstraintsCompPtr->IsTriggerModeSupported(TM_SOFTWARE)){
		SelectionEntry entry = {"Software", "Software trigger", TM_SOFTWARE};
		m_selectionList.push_back(entry);
	}
}


// reimplemented (iprm::IOptionsList)

int CTriggerParamsComp::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CTriggerParamsComp::GetOptionsCount() const
{
	return int(m_selectionList.size());
}


QString CTriggerParamsComp::GetOptionName(int index) const
{
	Q_ASSERT(int(m_selectionList.size()) > index);
	Q_ASSERT(index >= 0);

	return m_selectionList[index].name;
}


QString CTriggerParamsComp::GetOptionDescription(int index) const
{
	Q_ASSERT(int(m_selectionList.size()) > index);
	Q_ASSERT(index >= 0);

	return m_selectionList[index].description;
}


QByteArray CTriggerParamsComp::GetOptionId(int index) const
{
	Q_ASSERT(int(m_selectionList.size()) > index);
	Q_ASSERT(index >= 0);

	return m_selectionList[index].name.toLocal8Bit();
}


bool CTriggerParamsComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// reimplemented (icomp::CComponentBase)

void CTriggerParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_triggerMode = *m_triggerModeAttrPtr;

	m_triggersCount = *m_triggersCountAttrPtr;

	InitializeSelectionList();
}


} // namespace isig


