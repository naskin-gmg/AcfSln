#include "iwiz/CComposedStateControllerComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"


namespace iwiz
{


// public methods

CComposedStateControllerComp::CComposedStateControllerComp()
:	m_isStateEnabled(true),
	m_isStateActive(false)
{
}


// reimplemented (iproc::IStateController)

bool CComposedStateControllerComp::IsStateEnabled() const
{
	return m_isStateEnabled;
}


bool CComposedStateControllerComp::IsStateActive() const
{
	return m_isStateActive;
}


bool CComposedStateControllerComp::IsEnterAllowed(bool isActionAllowed, const IStateController* /*prevStatePtr*/) const
{
	if (m_isStateActive){
		return false;
	}

	if (isActionAllowed && m_processOnEnterCompPtr.IsValid()){
		if (		(m_processOnEnterCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnEnterCompPtr->AreParamsAccepted(this, NULL, NULL)){
			return false;
		}
	}

	if (m_slaveControllersCompPtr.IsValid()){
		int slavesCount = m_slaveControllersCompPtr.GetCount();
		for (int i = 0; i < slavesCount; ++i){
			const iproc::IStateController* slaveConstrollerPtr = m_slaveControllersCompPtr[i];
			if ((slaveConstrollerPtr != NULL) && !slaveConstrollerPtr->IsEnterAllowed(isActionAllowed)){
				return false;	// if any subcontroller block entering this state, it will be blocked
			}
		}
	}

	return true;
}


bool CComposedStateControllerComp::IsLeaveAllowed(bool isActionAllowed, const IStateController* /*nextStatePtr*/) const
{
	if (isActionAllowed && m_processOnLeaveCompPtr.IsValid()){
		if (		(m_processOnLeaveCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnLeaveCompPtr->AreParamsAccepted(this, NULL, NULL)){
			return false;
		}
	}

	if (m_slaveControllersCompPtr.IsValid()){
		int slavesCount = m_slaveControllersCompPtr.GetCount();
		for (int i = 0; i < slavesCount; ++i){
			const iproc::IStateController* slaveConstrollerPtr = m_slaveControllersCompPtr[i];
			if ((slaveConstrollerPtr != NULL) && !slaveConstrollerPtr->IsLeaveAllowed(isActionAllowed)){
				return false;	// if any subcontroller block leaving this state, it will be blocked
			}
		}
	}

	return true;
}


bool CComposedStateControllerComp::TryEnterState(bool isActionAllowed, const IStateController* /*prevStatePtr*/)
{
	if (m_isStateActive){
		return false;
	}

	if (isActionAllowed && m_processOnEnterCompPtr.IsValid()){
		if (		(m_processOnEnterCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnEnterCompPtr->AreParamsAccepted(this, NULL, NULL) ||
					(m_processOnEnterCompPtr->DoProcessing(this, NULL, NULL)  != iproc::IProcessor::TS_OK)){
			return false;
		}
	}

	ChangeSet changeSet(CF_STATE_ENTERED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	m_isStateActive = true;

	return true;
}


bool CComposedStateControllerComp::TryLeaveState(bool isActionAllowed, const IStateController* /*nextStatePtr*/)
{
	if (!m_isStateActive){
		return false;
	}

	if (isActionAllowed && m_processOnLeaveCompPtr.IsValid()){
		if (		(m_processOnLeaveCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnLeaveCompPtr->AreParamsAccepted(this, NULL, NULL) ||
					(m_processOnLeaveCompPtr->DoProcessing(this, NULL, NULL)  != iproc::IProcessor::TS_OK)){
			return false;
		}
	}

	ChangeSet changeSet(CF_STATE_LEAVED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	m_isStateActive = false;

	return true;
}

bool CComposedStateControllerComp::IsResetAllowed() const
{
	if (m_slaveControllersCompPtr.IsValid()){
		int slavesCount = m_slaveControllersCompPtr.GetCount();
		for (int i = 0; i < slavesCount; ++i){
			const iproc::IStateController* slaveConstrollerPtr = m_slaveControllersCompPtr[i];
			if ((slaveConstrollerPtr != NULL) && !slaveConstrollerPtr->IsResetAllowed()){
				return false;
			}
		}
	}

	return true;
}


// protected methods

void CComposedStateControllerComp::UpdateAllMembers()
{
	bool isEnabled = true;

	if (m_slaveControllersCompPtr.IsValid()){
		int slavesCount = m_slaveControllersCompPtr.GetCount();
		for (int i = 0; i < slavesCount; ++i){
			const iproc::IStateController* slaveConstrollerPtr = m_slaveControllersCompPtr[i];
			if (slaveConstrollerPtr != NULL){
				isEnabled = isEnabled && slaveConstrollerPtr->IsStateEnabled();
			}
		}
	}

	if (m_isStateEnabled != isEnabled){
		ChangeSet changeSet(CF_STATE_ENABLED);
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

		m_isStateEnabled = isEnabled;
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CComposedStateControllerComp::OnModelChanged(int /*modelId*/, const ChangeSet& changeSet)
{
	if (		changeSet.Contains(CF_STATE_ENTERED) ||
				changeSet.Contains(CF_STATE_LEAVED) ||
				changeSet.Contains(CF_GRAPH_CHANGED) ||
				changeSet.Contains(CF_STATE_ENABLED)){
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

		UpdateAllMembers();
	}
}


// reimplemented (icomp::CComponentBase)

void CComposedStateControllerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int modelsCount = m_slaveControllerModelsCompPtr.GetCount();
	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* parameterModelPtr = m_slaveControllerModelsCompPtr[i];
		if (parameterModelPtr != NULL){
			RegisterModel(parameterModelPtr, i);
		}
	}

	UpdateAllMembers();
}


void CComposedStateControllerComp::OnComponentDestroyed()
{
	BaseClass2::UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


