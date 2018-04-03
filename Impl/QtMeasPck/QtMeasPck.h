#ifndef QtMeasPck_included
#define QtMeasPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>
#include <ibase/TModelObserverCompWrap.h>

// ACF-Solutions includes
#include <iqtmeas/CDataSequenceViewComp.h>
#include <iqtmeas/CChartBaseDataSequenceViewComp.h>
#include <iqtmeas/CDataStatisticsGuiComp.h>
#include <iqtmeas/CNumericParamsGuiComp.h>
#include <iqtmeas/CDataSequenceSupplierResultsViewComp.h>


/**
	System-independent package for sampling, measurement acquisition and related thema.
*/
namespace QtMeasPck
{


typedef iqtmeas::CDataSequenceViewComp DataSequenceView;
typedef iqtmeas::CChartBaseDataSequenceViewComp ChartBaseDataSequenceView;
typedef iqtmeas::CDataStatisticsGuiComp DataStatisticsGui;
typedef ibase::TModelObserverCompWrap<iqtmeas::CNumericParamsGuiComp> NumericParamsGui;
typedef iqtmeas::CDataSequenceSupplierResultsViewComp DataSequenceSupplierResultsView;


} // namespace QtMeasPck


#endif // !QtMeasPck_included


