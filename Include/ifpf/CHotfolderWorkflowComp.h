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
	I_END_COMPONENT;

	CHotfolderWorkflowComp();

	// reimplemented (ifpf::IHotfolderInfoManager)
	virtual ifpf::IHotfolderProcessingInfo* GetProcessingInfo(const QString& hotfolderName) const;

	// reimplemented (ifpf::IHotfolderWorkflow)
	virtual ifpf::IHotfolderWorkflowItem* AddHotfolder(const QString& hotfolderName, const QString& hotfolderId = QString());
	virtual bool RemoveHotfolder(const QString& hotfolderName);
	virtual QStringList GetHotfolderIds() const;
	virtual QStringList GetHotfolderList() const;
	virtual ifpf::IHotfolderWorkflowItem* GetHotfolder(const QString& hotfolderName) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	struct HotfolderItem
	{
		istd::TDelPtr<iprm::IParamsSet> hotfolderParamsPtr;
		istd::TDelPtr<ifpf::IHotfolderWorkflowItem> elementPtr;
	};

	typedef istd::TPointerVector<HotfolderItem> Hotfolders;

	virtual HotfolderItem* CreateHotfolder(const QString& hotfolderName, const QString& hotfolderId = QString()) const;
	virtual ifpf::IHotfolderWorkflowItem* CreateWorkflowItem(const QString& hotfolderId) const;

private:
	I_MULTIFACT(iprm::IParamsSet, m_hotoflderFactoriesCompPtr);
	I_MULTIATTR(QString, m_hotoflderFactoryIdsAttrPtr);

	Hotfolders m_hotfolders;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderWorkflowComp_included

