// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef SignalPckPck_included
#define SignalPckPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <isig/CSamplingParamsComp.h>
#include <isig/CTriggerParamsComp.h>
#include <isig/CPulseParamsComp.h>


/**
	Base system-undependent general package.
*/
namespace SignalPck
{


typedef icomp::TModelCompWrap<isig::CSamplingParamsComp> SamplingParams;
typedef icomp::TModelCompWrap<isig::CTriggerParamsComp> TriggerParams;
typedef icomp::TModelCompWrap<isig::CPulseParamsComp> PulseParams;


} // namespace SignalPck


#endif // !SignalPckPck_included


