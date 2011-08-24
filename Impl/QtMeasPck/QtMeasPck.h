#ifndef QtMeasPck_included
#define QtMeasPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"
#include "iimg/CGeneralBitmap.h"

// ACF-Solutions includes
#include "iqtmeas/CDataSequenceViewComp.h"
#include "iqtmeas/CDataStatisticsGuiComp.h"
#include "iqtmeas/CNumericParamsGuiComp.h"


/**
	System-undependent package for sampling, measurement acquisition and related thema.
*/
namespace QtMeasPck
{


typedef iqtmeas::CDataSequenceViewComp DataSequenceView;
typedef iqtmeas::CDataStatisticsGuiComp DataStatisticsGui;
typedef iqtmeas::CNumericParamsGuiComp NumericParamsGui;


} // namespace QtMeasPck


#endif // !QtMeasPck_included


