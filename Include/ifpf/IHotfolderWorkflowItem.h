#ifndef ifpf_IHotfolderWorkflowItem_included
#define ifpf_IHotfolderWorkflowItem_included


// ACF includes
#include "iprm/INameParam.h"


namespace ifpf
{


/**
	Interface for getting some informations about properties of a given hotfolder.
*/
class IHotfolderWorkflowItem: virtual public iprm::INameParam
{
public:
	/**
		Get input direcories.
	*/
	virtual QStringList GetInputDirectories() const = 0;
	
	/**
		Get output directory.
	*/
	virtual QString GetOutputDirectory() const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderWorkflowItem_included


