#ifndef QwtPck_included
#define QwtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqwt/CProgressHistoryGuiComp.h"
#include "iqwt/CHistogramViewComp.h"

#include "iqwt3d/CSurfaceViewComp.h"


/**
	Qwt components package.
*/
namespace QwtPck
{


typedef iqwt::CProgressHistoryGuiComp ProgressHistoryGui;
typedef iqwt::CHistogramViewComp HistogramView;

typedef iqwt3d::CSurfaceViewComp SurfaceView;


} // namespace QwtPck


#endif // !QwtPck_included


