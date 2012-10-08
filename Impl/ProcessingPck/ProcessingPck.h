#ifndef ProcessingPck_included
#define ProcessingPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iproc/CCascadedProcessorComp.h"
#include "iproc/CIterativeProcessorComp.h"
#include "iproc/CModelBasedProcessingTriggerComp.h"


/**
	General processing package.
*/
namespace ProcessingPck
{


typedef iproc::CCascadedProcessorComp CascadedProcessor;
typedef iproc::CIterativeProcessorComp IterativeProcessor;
typedef iproc::CModelBasedProcessingTriggerComp ModelBasedProcessingTrigger;


} // namespace ProcessingPck


#endif // !ProcessingPck_included


