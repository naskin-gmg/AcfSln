// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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


