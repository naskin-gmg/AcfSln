#ifndef ifpf_CHotfolderWorkflowComp_included
#define ifpf_CHotfolderWorkflowComp_included


// STL includes
#include <map>


// ACF includes
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"
#include "istd/TSmartPtr.h"
#include "istd/TChangeDelegator.h"

#include "isys/ICriticalSection.h"

#include "icomp/CComponentBase.h"


// AcfSln includes
#include "ifpf/IHotfolderWorkflow.h"
#include "ifpf/CHotfolderProcessingItem.h"
#include "ifpf/CMonitoringSession.h"


namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolderWorkflowComp: public icomp::CComponentBase, virtual public ifpf::IHotfolderWorkflow
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CHotfolderWorkflowComp);
		I_REGISTER_INTERFACE(ifpf::IHotfolderWorkflow);
		I_REGISTER_INTERFACE(iser::ISerializable);

		I_ASSIGN_MULTI_0(m_hotoflderFactoriesCompPtr, "HotfolderFactories", "List of factories for hotfolder creation", true);
		I_ASSIGN_MULTI_0(m_hotoflderFactoryIdsAttrPtr, "HotfolderFactoryIds", "List of factory IDs for hotfolder creation", true);
	I_END_COMPONENT();

	CHotfolderWorkflowComp();

	// reimplemented (ifpf::IHotfolderWorkflow)
	virtual iprm::IParamsSet* AddHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId = istd::CString());
	virtual bool RemoveHotfolder(const istd::CString& hotfolderName);
	virtual ifpf::IHotfolderProcessingInfo* GetHotfolderProcessingInfo(const istd::CString& hotfolderName) const;
	virtual istd::CStringList GetHotfolderList() const;
	virtual int GetWorkingState(const istd::CString& hotfolderName) const;
	virtual istd::CStringList GetHotfolderIds() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual iprm::IParamsSet* CreateHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId = istd::CString()) const;

private:
	struct HotfolderInfo
	{
		istd::TDelPtr<iprm::IParamsSet> hotfolderPtr;
		istd::CString hotfolderId;
	};

	typedef std::vector<HotfolderInfo> Hotfolders;

	I_MULTIFACT(iprm::IParamsSet, m_hotoflderFactoriesCompPtr);
	I_MULTIATTR(istd::CString, m_hotoflderFactoryIdsAttrPtr);

	Hotfolders m_hotfolders;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderWorkflowComp_included

