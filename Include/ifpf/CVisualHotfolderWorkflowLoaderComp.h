#ifndef ifpf_CHotfolderWorkflowComp_included
#define ifpf_CHotfolderWorkflowComp_included


// STL includes
#include <map>


// ACF includes
#include "istd/TDelPtr.h"
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
	I_END_COMPONENT();

	CHotfolderWorkflowComp();

	// reimplemented (ifpf::IHotfolderWorkflow)
	virtual ifpf::IHotfolderProcessingInfo* AddHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId = istd::CString());
	virtual bool RemoveHotfolder(const istd::CString& hotfolderName);
	virtual ifpf::IHotfolderProcessingInfo* GetHotfolder(const istd::CString& hotfolderName) const;
	virtual istd::CStringList GetHotfoldersList() const;
	virtual int GetWorkingState(const istd::CString& hotfolderName) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual ifpf::IHotfolderProcessingInfo* CreateHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId = istd::CString()) const;

private:
	struct HotfolderInfo
	{
		istd::TDelPtr<ifpf::IHotfolderProcessingInfo> m_hotfolderPtr;
		istd::CString hotfolderId;
	};

	I_MULTIFACT(ifpf::IHotfolderProcessingInfo, m_hotoflderFactoriesCompPtr);
	I_MULTIATTR(istd::CString, m_hotoflderFactoryIdsAttrPtr);
};


} // namespace ifpf


#endif // !ifpf_CHotfolderWorkflowComp_included

