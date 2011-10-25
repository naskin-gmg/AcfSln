#ifndef InspectionPck_included
#define InspectionPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iinsp/CInspectionTaskComp.h"
#include "iinsp/CInspectionComp.h"


/**
	Base system-undependent general package.
*/
namespace InspectionPck
{


typedef icomp::TModelCompWrap<iinsp::CInspectionTaskComp> InspectionTask;
typedef icomp::TModelCompWrap<iinsp::CInspectionComp> Inspection;


} // namespace InspectionPck


#endif // !InspectionPck_included


