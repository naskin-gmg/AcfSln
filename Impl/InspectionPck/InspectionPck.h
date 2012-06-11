#ifndef InspectionPck_included
#define InspectionPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iinsp/CInspectionTaskComp.h"
#include "iinsp/CInformationProviderCacheComp.h"
#include "iinsp/CIdProviderCacheComp.h"


/**
	Base system-undependent general package.
*/
namespace InspectionPck
{


typedef icomp::TModelCompWrap<iinsp::CInspectionTaskComp> InspectionTask;
typedef icomp::TModelCompWrap<iinsp::CInformationProviderCacheComp> InformationProviderCache;
typedef icomp::TModelCompWrap<iinsp::CIdProviderCacheComp> IdProviderCache;


} // namespace InspectionPck


#endif // !InspectionPck_included


