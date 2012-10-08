#ifndef ihotf_IHotfolderInfoManager_included
#define ihotf_IHotfolderInfoManager_included


// ACF includes
#include "istd/IPolymorphic.h"
#include <QtCore/QString>


// AcfSln includes
#include "ihotf/IHotfolderProcessingInfo.h"


namespace ihotf
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
	virtual ihotf::IHotfolderProcessingInfo* GetProcessingInfo(const QString& hotfolderName) const = 0;

};


} // namespace ihotf


#endif // !ihotf_IHotfolderInfoManager_included


