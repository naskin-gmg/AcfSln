// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iauth/CRightsBasedEnablerComp.h>


namespace iauth
{


// public methods

CRightsBasedEnablerComp::CRightsBasedEnablerComp()
	:m_isEnabled(false)
{
}


// reimplemented (iprm::IEnableableParam)

bool CRightsBasedEnablerComp::IsEnabled() const
{
	return m_isEnabled;
}


bool CRightsBasedEnablerComp::IsEnablingAllowed() const
{
	return false;
}


bool CRightsBasedEnablerComp::SetEnabled(bool /*isEnabled*/)
{
	return false;
}


// reimplemented (iser::ISerializable)

bool CRightsBasedEnablerComp::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CRightsBasedEnablerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_rightsProviderModelCompPtr.IsValid()){
		m_rightsProviderModelCompPtr->AttachObserver(this);
	}
}


void CRightsBasedEnablerComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::TSingleModelObserverBase)

void CRightsBasedEnablerComp::OnUpdate(const ChangeSet& /*changeSet*/)
{
	bool retVal = m_isEnabled;

	if (m_rightsProviderCompPtr.IsValid()){
		int rightsCount = m_rightIdsAttrPtr.GetCount();

		retVal = (rightsCount > 0);
	
		for (int rightIndex = 0; rightIndex < rightsCount; rightIndex++){
			retVal = retVal && m_rightsProviderCompPtr->HasRight(m_rightIdsAttrPtr[rightIndex], true);
		}
	}

	if (retVal != m_isEnabled){
		istd::CChangeNotifier changePtr(this);

		m_isEnabled = retVal;
	}
}


} // namespace iauth


