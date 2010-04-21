#ifndef ifpf_IHotfolderWorkflow_included
#define ifpf_IHotfolderWorkflow_included


// ACF includes
#include "iser/ISerializable.h"

#include "iprm/IParamsSet.h"


// AcfSln includes
#include "ifpf/IHotfolderWorkflowItem.h"


namespace ifpf
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
	virtual ifpf::IHotfolderWorkflowItem* AddHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId = istd::CString()) = 0;

	/**	
		Remove a hotfolder with the name \hotfolderName from the workflow.
	*/
	virtual bool RemoveHotfolder(const istd::CString& hotfolderName) = 0;

	/**
		Get the list of possible hotfolder IDs.
	*/
	virtual istd::CStringList GetHotfolderIds() const = 0;

	/**
		Get hotfolder list in this workflow.
	*/
	virtual istd::CStringList GetHotfolderList() const = 0;

	/**
		Get the hotfolder.
	*/
	virtual ifpf::IHotfolderWorkflowItem* GetHotfolder(const istd::CString& hotfolderName) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderWorkflow_included


