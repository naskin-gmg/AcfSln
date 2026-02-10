// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iproc/CModelBasedProcessingTriggerComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <imod/IModel.h>
#include <ibase/IProgressManager.h>


namespace iproc
{


// public methods

CModelBasedProcessingTriggerComp::CModelBasedProcessingTriggerComp()
:	m_paramsObserver(this),
	m_isLocked(false)
{
}

// reimplemented (icomp::CComponentBase)

void CModelBasedProcessingTriggerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_paramsSetModelCompPtr.IsValid() && *m_triggerOnParamsChangeAttrPtr){
		m_paramsSetModelCompPtr->AttachObserver(&m_paramsObserver);
	}
}


void CModelBasedProcessingTriggerComp::OnComponentDestroyed()
{
	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// protected methods
	
// reimplemented (imod::CSingleModelObserverBase)

void CModelBasedProcessingTriggerComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (!m_isLocked){
		DoProcessing();
	}
}


// private methods

void CModelBasedProcessingTriggerComp::DoProcessing()
{
	if (!m_processorCompPtr.IsValid()){
		SendErrorMessage(0, "Processor not set");

		return;
	}

	if (!m_outputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Output data object not set");

		return;
	}

	m_isLocked = true;

	istd::IChangeable* inputDataPtr = dynamic_cast<istd::IChangeable*>(GetObservedModel());

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDataPtr,
				m_outputDataCompPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());

	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing failed");
	}

	m_isLocked = false;
}


// public methods of the embedded class ParamsObserver

CModelBasedProcessingTriggerComp::ParamsObserver::ParamsObserver(CModelBasedProcessingTriggerComp* parentPtr)
:	m_parent(*parentPtr)
{
}


// reimplemented (imod::CSingleModelObserverBase)

void CModelBasedProcessingTriggerComp::ParamsObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.DoProcessing();
}


} // namespace iproc


