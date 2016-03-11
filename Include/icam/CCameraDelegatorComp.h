#ifndef _qcam_CCameraDelegatorBase_included
#define _qcam_CCameraDelegatorBase_included


// ACF includes
#include "ilog/TLoggerCompWrap.h"
#include "iprm/IParamsSet.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"


namespace icam
{


/**
	Implementation of bitmap acquisition delegating all calls to some slave object.
	It is used to provide cascades of camera implementations.
*/
class CCameraDelegatorComp:
			public ilog::CLoggerComponentBase,
			public IBitmapAcquisition
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCameraDelegatorComp);
		I_REGISTER_INTERFACE(IBitmapAcquisition);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_ASSIGN(m_slaveCameraCompPtr, "SlaveCamera", "Slave camera object", true, "SlaveCamera");
		I_ASSIGN(m_parameterSetCompPtr, "ParamsSet", "Parameter set used for overriding of the input parameters", false, "ParamsSet");
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
				ibase::IProgressManager* progressManagerPtr = NULL);
	virtual int BeginTask(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);
	virtual int WaitTaskFinished(
					int taskId = -1,
					double timeoutTime = -1,
					bool killOnTimeout = true);
	virtual void CancelTask(int taskId = -1);
	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

protected:
	virtual const iprm::IParamsSet* GetWorkingParamsSet(const iprm::IParamsSet* inputParamsPtr) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	I_REF(IBitmapAcquisition, m_slaveCameraCompPtr);
	I_REF(iprm::IParamsSet, m_parameterSetCompPtr);
};


} // namespace icam


#endif // !_qcam_CCameraDelegatorBase_included


