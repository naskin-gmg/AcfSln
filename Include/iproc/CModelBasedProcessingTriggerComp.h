#ifndef iproc_CModelBasedProcessingTriggerComp_included
#define iproc_CModelBasedProcessingTriggerComp_included


// ACF includes
#include "ilog/TLoggerCompWrap.h"

#include "imod/TSingleModelObserverBase.h"

#include "iproc/IProcessor.h"


namespace iproc
{


/**
	Component to trigger a processing action depending on model changes.
*/
class CModelBasedProcessingTriggerComp:
			public ilog::CLoggerComponentBase,
			public imod::CSingleModelObserverBase
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;
	
	I_BEGIN_COMPONENT(CModelBasedProcessingTriggerComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_processorCompPtr, "Processor", "Target processing component", true, "Processor");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress manager", false, "ProgressManager");
		I_ASSIGN(m_paramsSetCompPtr, "ProcessingParamsSet", "Processing parameters", false, "ProcessingParameters");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Processing output data", true, "OutputData");
		I_ASSIGN(m_triggerOnParamsChangeAttrPtr, "TriggerOnParameterChanges", "Start processing if the parameters have been changed", true, false);
	I_END_COMPONENT;

	CModelBasedProcessingTriggerComp();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

private:
	void DoProcessing();

private:
	/**
		\internal
		Observes changes of directory to force update if it has changed.
	*/
	class ParamsObserver: public imod::TSingleModelObserverBase<iprm::IParamsSet>
	{
	public:
		ParamsObserver(CModelBasedProcessingTriggerComp* parentPtr);

		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		CModelBasedProcessingTriggerComp& m_parent;
	};

private:
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(ibase::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_ATTR(bool, m_triggerOnParamsChangeAttrPtr);

	ParamsObserver m_paramsObserver;
};


} // namespace iproc


#endif // !iproc_CModelBasedProcessingTriggerComp_included


