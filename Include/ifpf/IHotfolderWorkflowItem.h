#ifndef ifpf_IHotfolderWorkflowItem_included
#define ifpf_IHotfolderWorkflowItem_included


// ACF includes
#include "istd/INamed.h"


namespace ifpf
{


/**
	Interface for getting some informations about properties of a given hotfolder.
*/
class IHotfolderWorkflowItem: virtual public istd::INamed
{
public:
	/**
		Get input direcories.
	*/
	virtual istd::CStringList GetInputDirectories() const = 0;
	
	/**
		Get output directory.
	*/
	virtual istd::CString GetOutputDirectory() const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderWorkflowItem_included


