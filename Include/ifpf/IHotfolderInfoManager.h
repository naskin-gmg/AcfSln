#ifndef ifpf_IHotfolderInfoManager_included
#define ifpf_IHotfolderInfoManager_included


// ACF includes
#include "istd/IPolymorphic.h"
#include <QString>


// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"


namespace ifpf
{


/**
	Interface for getting some infos about a hotfolder.
*/
class IHotfolderInfoManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get processing info for a given hotfolder.
	*/
	virtual ifpf::IHotfolderProcessingInfo* GetProcessingInfo(const QString& hotfolderName) const = 0;

};


} // namespace ifpf


#endif // !ifpf_IHotfolderInfoManager_included


