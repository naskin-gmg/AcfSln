// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef QtMeasPck_included
#define QtMeasPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>
#include <ibase/TModelObserverCompWrap.h>

// ACF-Solutions includes
#include <iqtmeas/CDataSequenceViewComp.h>
#include <iqtmeas/CDataStatisticsGuiComp.h>
#include <iqtmeas/CNumericParamsGuiComp.h>
#include <iqtmeas/CDataSequenceSupplierResultsViewComp.h>


/**
	System-independent package for sampling, measurement acquisition and related thema.
*/
namespace QtMeasPck
{


typedef iqtmeas::CDataSequenceViewComp DataSequenceView;
typedef iqtmeas::CDataStatisticsGuiComp DataStatisticsGui;
typedef ibase::TModelObserverCompWrap<iqtmeas::CNumericParamsGuiComp> NumericParamsGui;
typedef iqtmeas::CDataSequenceSupplierResultsViewComp DataSequenceSupplierResultsView;


} // namespace QtMeasPck


#endif // !QtMeasPck_included


