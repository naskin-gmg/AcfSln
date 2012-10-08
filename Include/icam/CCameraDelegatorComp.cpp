#include "icam/CCameraDelegatorComp.h"


// ACF includes
#include "istd/CIndex2d.h"


namespace icam
{


// reimplemented (iipr::IBitmapAcquisition)

istd::CIndex2d CCameraDelegatorComp::GetBitmapSize(const iprm::IParamsSet* paramsPtr) const
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->GetBitmapSize(paramsPtr);
	}

	return istd::CIndex2d(0, 0);
}


// reimplemented (iproc::IProcessor)

int CCameraDelegatorComp::GetProcessorState(const iprm::IParamsSet* paramsPtr) const
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->GetProcessorState(paramsPtr);
	}

	return PS_UNKNOWN;
}


bool CCameraDelegatorComp::AreParamsAccepted(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			const istd::IChangeable* outputPtr) const
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->AreParamsAccepted(paramsPtr, inputPtr, outputPtr);
	}

	return false;
}


int CCameraDelegatorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->DoProcessing(paramsPtr, inputPtr, outputPtr, progressManagerPtr);
	}

	return TS_INVALID;
}


int CCameraDelegatorComp::BeginTask(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->BeginTask(paramsPtr, inputPtr, outputPtr, progressManagerPtr);
	}

	return -1;
}


int CCameraDelegatorComp::WaitTaskFinished(
				int taskId,
				double timeoutTime,
				bool killOnTimeout)
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->WaitTaskFinished(taskId, timeoutTime, killOnTimeout);
	}

	return TS_INVALID;
}


void CCameraDelegatorComp::CancelTask(int taskId)
{
	if (m_slaveCameraCompPtr.IsValid()){
		m_slaveCameraCompPtr->CancelTask(taskId);
	}
}


int CCameraDelegatorComp::GetReadyTask()
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->GetReadyTask();
	}

	return -1;
}


int CCameraDelegatorComp::GetTaskState(int taskId) const
{
	if (m_slaveCameraCompPtr.IsValid()){
		return m_slaveCameraCompPtr->GetTaskState(taskId);
	}

	return TS_INVALID;
}


void CCameraDelegatorComp::InitProcessor(const iprm::IParamsSet* paramsPtr)
{
	if (m_slaveCameraCompPtr.IsValid()){
		m_slaveCameraCompPtr->InitProcessor(paramsPtr);
	}
}


} // namespace icam


