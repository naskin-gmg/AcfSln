#ifndef ifpf_IHotfolderWorkflow_included
#define ifpf_IHotfolderWorkflow_included


// ACF includes
#include "istd/INamed.h"

// AcfSln includes
#include "ifpf/IHotfolderParams.h"


namespace ifpf
{


/**
	Common interface for a hotfolder workflow static model.
*/
class IHotfolderWorkflow: virtual public istd::INamed
{
public:
	typedef std::vector<ifpf::IHotfolderParams*> Hotfolders;

	/**	
		Add a hotfolder to the workflow.
		The name of the hotfolder must be unique, otherwise no folder will be added an the function returns \cn null.
	*/
	virtual ifpf::IHotfolderParams* AddHotfolder(const istd::CString& hotfolderName) = 0;

	/**	
		Remove a hotfolder with the name \hotfolderName from the workflow.
	*/
	virtual bool RemoveHotfolder(const istd::CString& hotfolderName) = 0;

	/**
		Get the specified hotfolder instance.
	*/
	virtual ifpf::IHotfolderParams* GetHotfolder(const istd::CString& hotfolderName) const = 0;

	/**
		Get workflow hotfolder list.
	*/
	virtual istd::CStringList GetHotfoldersList() const = 0;

	/**
		Get processing state of the hotfolder \c hotfolderName.
	*/
	virtual int GetProcessingState(const istd::CString& hotfolderName) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderWorkflow_included


