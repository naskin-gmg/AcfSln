#ifndef CalibrationPck_included
#define CalibrationPck_included


// ACF includes
#include <imod/IObserver.h>
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

// ACF-Solutions includes
#include <icalib/CAffineCalibration2dComp.h>
#include <icalib/CPerspectiveCalibration2dComp.h>
#include <icalib/CSimpleLensCorrectionComp.h>
#include <icalib/CStaticCalibrationProviderComp.h>
#include <icalib/CCalibration2dProxyComp.h>
#include <icalib/CAffineCalibrationProviderComp.h>


/**
	Base system-independent general package.
*/
namespace CalibrationPck
{


typedef icomp::TModelCompWrap<icalib::CAffineCalibration2dComp> AffineCalibration2d;
typedef icomp::TModelCompWrap<icalib::CPerspectiveCalibration2dComp> PerspectiveCalibration2d;
typedef icomp::TModelCompWrap<icalib::CSimpleLensCorrectionComp> SimpleLensCorrection;
typedef icomp::TModelCompWrap<icalib::CStaticCalibrationProviderComp> StaticCalibrationProvider;
typedef icomp::TModelCompWrap<icalib::CCalibration2dProxyComp> Calibration2dProxy;
typedef icomp::TModelCompWrap<icalib::CAffineCalibrationProviderComp> AffineCalibrationProvider;


} // namespace CalibrationPck


#endif // !CalibrationPck_included


