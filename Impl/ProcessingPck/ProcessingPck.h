// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ProcessingPck_included
#define ProcessingPck_included


#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iproc/CCascadedProcessorComp.h>
#include <iproc/CIterativeProcessorComp.h>
#include <iproc/CModelBasedProcessingTriggerComp.h>


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


