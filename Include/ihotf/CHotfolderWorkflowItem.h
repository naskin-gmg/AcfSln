#ifndef ihotf_CHotfolderWorkflowItem_included
#define ihotf_CHotfolderWorkflowItem_included


// ACF includes
#include "iprm/CNameParam.h"


// AcfSln includes
#include "ihotf/IHotfolderWorkflowItem.h"
#include "ihotf/IHotfolderInfoManager.h"


namespace ihotf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolderWorkflowItem:
			public iprm::CNameParam,
			virtual public ihotf::IHotfolderWorkflowItem
{
public:
	typedef iprm::CNameParam BaseClass;

	void Initialize(const QString& hotfolderId,
					const ihotf::IHotfolderInfoManager* infoManagerPtr);

	/**
		Get hotfolder's ID
	*/
	virtual QString GetHotfolderId() const;

	// reimplemented (ihotf::IHotfolderWorkflowItem)
	virtual QStringList GetInputDirectories() const;
	virtual QString GetOutputDirectory() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QString m_hotfolderId;

	const ihotf::IHotfolderInfoManager* m_infoManagerPtr;
};


} // namespace ihotf


#endif // !ihotf_CHotfolderWorkflowItem_included

