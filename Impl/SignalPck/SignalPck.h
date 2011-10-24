#ifndef SignalPckPck_included
#define SignalPckPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "isig/CSamplingParamsComp.h"
#include "isig/CTriggerParamsComp.h"


/**
	Base system-undependent general package.
*/
namespace SignalPck
{


typedef icomp::TModelCompWrap<isig::CSamplingParamsComp> SamplingParams;
typedef icomp::TModelCompWrap<isig::CTriggerParamsComp> TriggerParams;


} // namespace SignalPck


#endif // !SignalPckPck_included


