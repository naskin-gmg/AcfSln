// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icam/CProcessedCameraComp.h>


namespace icam
{


// public methods

// reimplemented (iproc::IProcessor)

int CProcessedCameraComp::BeginTask(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	BaseClass::InitProcessor(paramsPtr);

	const iprm::IParamsSet* workingParamsPtr = GetWorkingParamsSet(paramsPtr);

	if (m_preProcessorCompPtr.IsValid()){
		m_preProcessorCompPtr->InitProcessor(workingParamsPtr);

		m_preProcessorCompPtr->BeginTask(workingParamsPtr, inputPtr, outputPtr, progressManagerPtr);
	}

	int retVal = -1;
	if (m_slaveCameraCompPtr.IsValid()){
		retVal = BaseClass::BeginTask(paramsPtr, inputPtr, outputPtr, progressManagerPtr);
	}

	if (m_postProcessorCompPtr.IsValid() && BaseClass::GetTaskState(retVal) == TS_OK){
		m_postProcessorCompPtr->InitProcessor(workingParamsPtr);

		m_postProcessorCompPtr->BeginTask(workingParamsPtr, outputPtr, outputPtr, progressManagerPtr);
	}

	return retVal;
}


iproc::IProcessor::TaskState CProcessedCameraComp::DoProcessing(
		const iprm::IParamsSet* paramsPtr,
		const istd::IPolymorphic* inputPtr,
		istd::IChangeable* outputPtr,
		ibase::IProgressManager* progressManagerPtr)
{
	const iprm::IParamsSet* workingParamsPtr = GetWorkingParamsSet(paramsPtr);

	BaseClass::InitProcessor(paramsPtr);

	if (m_preProcessorCompPtr.IsValid()){
		m_preProcessorCompPtr->InitProcessor(workingParamsPtr);

		m_preProcessorCompPtr->DoProcessing(workingParamsPtr, inputPtr, outputPtr, progressManagerPtr);
	}

	iproc::IProcessor::TaskState retVal = TS_OK;
	if (m_slaveCameraCompPtr.IsValid()){
		retVal = BaseClass::DoProcessing(paramsPtr, inputPtr, outputPtr, progressManagerPtr);
	}

	if (m_postProcessorCompPtr.IsValid() && retVal == TS_OK){
		m_postProcessorCompPtr->InitProcessor(workingParamsPtr);

		m_postProcessorCompPtr->DoProcessing(workingParamsPtr, outputPtr, outputPtr, progressManagerPtr);
	}

	return retVal;
}


} // namespace icam


