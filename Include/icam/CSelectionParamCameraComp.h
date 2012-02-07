#ifndef iprm_CSelectionParamCameraComp_included
#define iprm_CSelectionParamCameraComp_included


// STL includes
#include <map>

// ACF includes
#include "istd/TDelPtr.h"
#include "istd/CString.h"
#include "iprm/IParamsManager.h"

// ACF-Solutions include
#include "icam/CCameraDelegatorBase.h"


namespace icam
{


class CSelectionParamCameraComp: public CCameraDelegatorBase
{
public:
	typedef CCameraDelegatorBase BaseClass;

	I_BEGIN_COMPONENT(CSelectionParamCameraComp);
		I_ASSIGN(m_selectionIdAttrPtr, "SelectionId", "ID of selection in parameter set", true, "Input");
		I_ASSIGN(m_defaultIndexAttrPtr, "DefaultIndex", "Default selected index, if negative no selected parameter ID will be used", false, 0);
		I_ASSIGN(m_paramsManagerCompPtr, "ParamsManager", "Manager of parameter set used to realize selection", true, "ParamsManager");
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
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

protected:
	const iprm::IParamsSet* CreateParamsSet(const iprm::IParamsSet* paramsPtr) const;

	class JoinParamsSet: virtual public iprm::IParamsSet
	{
	public:
		JoinParamsSet(const iprm::IParamsSet* origParamsPtr, const iprm::IParamsSet* selectedParamsPtr);

		// reimplemented (iprm::IParamsSet)
		virtual const iser::ISerializable* GetParameter(const std::string& id) const;
		virtual iser::ISerializable* GetEditableParameter(const std::string& id);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	private:
		const iprm::IParamsSet* m_origParamsPtr;
		const iprm::IParamsSet* m_selectedParamsPtr;
	};

private:
	I_ATTR(std::string, m_selectionIdAttrPtr);
	I_ATTR(int, m_defaultIndexAttrPtr);
	I_REF(iprm::IParamsManager, m_paramsManagerCompPtr);

	typedef istd::TDelPtr<const iprm::IParamsSet> ParamSetPtr;
	/**
		Map task ID to parameter set.
	*/
	typedef std::map<int, ParamSetPtr> ParamsMap;

	ParamsMap m_paramsMap;
};


} // namespace icam


#endif // !iprm_CSelectionParamCameraComp_included


