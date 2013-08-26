#ifndef ihotf_IHotfolderWorkflow_included
#define ihotf_IHotfolderWorkflow_included


// ACF includes
#include "iser/ISerializable.h"

#include "iprm/IParamsSet.h"


// AcfSln includes
#include "ihotf/IHotfolderWorkflowItem.h"


namespace ihotf
{


/**
	Common interface for a hotfolder workflow static model.
*/
class IHotfolderWorkflow: virtual public iser::ISerializable
{
public:
	enum ChangeFlags
	{
		CF_HOTFOLDER_ADDED = 0x100000,
		CF_HOTFOLDER_REMOVED = 0x200000
	};

	/**
		Add a hotfolder to the workflow.
		The name of the hotfolder must be unique, otherwise no folder will be added an the function returns \c null.
	*/
	virtual ihotf::IHotfolderWorkflowItem* AddHotfolder(const QString& hotfolderName, const QString& hotfolderId = QString()) = 0;

	/**	
		Remove a hotfolder with the name \c hotfolderName from the workflow.
	*/
	virtual bool RemoveHotfolder(const QString& hotfolderName) = 0;

	/**
		Get the list of possible hotfolder IDs.
	*/
	virtual QStringList GetHotfolderIds() const = 0;

	/**
		Get hotfolder list in this workflow.
	*/
	virtual QStringList GetHotfolderList() const = 0;

	/**
		Get the hotfolder.
	*/
	virtual ihotf::IHotfolderWorkflowItem* GetHotfolder(const QString& hotfolderName) const = 0;
};


} // namespace ihotf


#endif // !ihotf_IHotfolderWorkflow_included


