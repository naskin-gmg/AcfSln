#ifndef ifpf_CHotfolderWorkflowItem_included
#define ifpf_CHotfolderWorkflowItem_included


// ACF includes
#include "iprm/CNameParam.h"


// AcfSln includes
#include "ifpf/IHotfolderWorkflowItem.h"
#include "ifpf/IHotfolderInfoManager.h"


namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolderWorkflowItem:
			public iprm::CNameParam,
			virtual public ifpf::IHotfolderWorkflowItem
{
public:
	typedef iprm::CNameParam BaseClass;

	void Initialize(const QString& hotfolderId,
					const ifpf::IHotfolderInfoManager* infoManagerPtr);

	/**
		Get hotfolder's ID
	*/
	virtual QString GetHotfolderId() const;

	// reimplemented (ifpf::IHotfolderWorkflowItem)
	virtual QStringList GetInputDirectories() const;
	virtual QString GetOutputDirectory() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QString m_hotfolderId;

	const ifpf::IHotfolderInfoManager* m_infoManagerPtr;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderWorkflowItem_included

