#ifndef ProductionPck_included
#define ProductionPck_included


#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iprod/CProductionHistoryComp.h>


/**
	Production-related component package
*/
namespace ProductionPck
{


typedef icomp::TModelCompWrap<iprod::CProductionHistoryComp> ProductionHistory;


} // namespace ProductionPck


#endif // !ProductionPck_included


