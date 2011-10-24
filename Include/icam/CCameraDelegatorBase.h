#ifndef _qcam_CCameraDelegatorBase_included
#define _qcam_CCameraDelegatorBase_included


// ACF includes
#include "icomp/CComponentBase.h"

// IACF includes
#include "iproc/IBitmapAcquisition.h"


namespace icam
{


/**
	Implementation of bitmap acquisition delegating all calls to some slave object.
	It is used to provide cascades of camera implementations.
*/
class CCameraDelegatorBase:
			public icomp::CComponentBase,
			public iproc::IBitmapAcquisition
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCameraDelegatorBase);
		I_REGISTER_INTERFACE(iproc::IBitmapAcquisition);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_ASSIGN(m_slaveCameraCompPtr, "SlaveCamera", "Slave camera object", true, "SlaveCamera");
	I_END_COMPONENT;

	// reimplemented (iipr::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int GetProcessorState(const iprm::IParamsSet* paramsPtr) const;
	virtual bool AreParamsAccepted(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				const istd::IChangeable* outputPtr) const;
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);
	virtual int BeginTask(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);
	virtual int WaitTaskFinished(
					int taskId = -1,
					double timeoutTime = -1,
					bool killOnTimeout = true);
	virtual void CancelTask(int taskId = -1);
	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

protected:
	I_REF(iproc::IBitmapAcquisition, m_slaveCameraCompPtr);
};


} // namespace icam


#endif // !_qcam_CCameraDelegatorBase_included


