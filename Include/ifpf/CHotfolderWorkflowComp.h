#ifndef ifpf_CHotfolderWorkflowComp_included
#define ifpf_CHotfolderWorkflowComp_included


// STL includes
#include <map>


// ACF includes
#include "istd/TDelPtr.h"
#include "istd/TChangeDelegator.h"
#include "istd/TPointerVector.h"

#include "isys/ICriticalSection.h"

#include "icomp/CComponentBase.h"


// AcfSln includes
#include "ifpf/IHotfolderWorkflow.h"
#include "ifpf/IHotfolderInfoManager.h"
#include "ifpf/CHotfolderWorkflowItem.h"



namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolderWorkflowComp:
				public icomp::CComponentBase,
				virtual public ifpf::IHotfolderWorkflow,
				virtual public ifpf::IHotfolderInfoManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CHotfolderWorkflowComp);
		I_REGISTER_INTERFACE(ifpf::IHotfolderWorkflow);
		I_REGISTER_INTERFACE(ifpf::IHotfolderInfoManager);
		I_REGISTER_INTERFACE(iser::ISerializable);

		I_ASSIGN_MULTI_0(m_hotoflderFactoriesCompPtr, "HotfolderFactories", "List of factories for hotfolder creation", true);
		I_ASSIGN_MULTI_0(m_hotoflderFactoryIdsAttrPtr, "HotfolderFactoryIds", "List of factory IDs for hotfolder creation", true);
	I_END_COMPONENT();

	CHotfolderWorkflowComp();

	// reimplemented (ifpf::IHotfolderInfoManager)
	virtual ifpf::IHotfolderProcessingInfo* GetProcessingInfo(const istd::CString& hotfolderName) const;

	// reimplemented (ifpf::IHotfolderWorkflow)
	virtual ifpf::IHotfolderWorkflowItem* AddHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId = istd::CString());
	virtual bool RemoveHotfolder(const istd::CString& hotfolderName);
	virtual istd::CStringList GetHotfolderIds() const;
	virtual istd::CStringList GetHotfolderList() const;
	virtual ifpf::IHotfolderWorkflowItem* GetHotfolder(const istd::CString& hotfolderName) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	struct HotfolderItem
	{
		istd::TDelPtr<iprm::IParamsSet> hotfolderParamsPtr;
		istd::TDelPtr<ifpf::IHotfolderWorkflowItem> elementPtr;
	};

	typedef istd::TPointerVector<HotfolderItem> Hotfolders;

	virtual HotfolderItem* CreateHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId = istd::CString()) const;
	virtual ifpf::IHotfolderWorkflowItem* CreateWorkflowItem(const istd::CString& hotfolderId) const;

private:
	I_MULTIFACT(iprm::IParamsSet, m_hotoflderFactoriesCompPtr);
	I_MULTIATTR(istd::CString, m_hotoflderFactoryIdsAttrPtr);

	Hotfolders m_hotfolders;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderWorkflowComp_included

