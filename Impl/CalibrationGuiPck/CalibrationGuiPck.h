#ifndef CalibrationGuiPck_included
#define CalibrationGuiPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <ibase/TMakeModelObserverCompWrap.h>

// ACF-Solutions includes
#include <icalibgui/CNoneCalibrationShape.h>
#include <icalibgui/CAffiniteCalibrationShape.h>
#include <icalibgui/CPerspectiveCalibrationShape.h>
#include <icalibgui/CPerspectiveCalibEditorComp.h>
#include <icalibgui/CSimpleLensCorrectionEditorComp.h>


/**
	Qt scene visualization package.
*/
namespace CalibrationGuiPck
{


typedef icomp::TMakeComponentWrap<icalibgui::CNoneCalibrationShape, iview::IShape, imod::IObserver, iview::IVisualizable, iview::ITouchable> NoneCalibrationShape;
typedef ibase::TMakeModelObserverCompWrap<icalibgui::CAffiniteCalibrationShape, iview::IShape, imod::IObserver, iview::IVisualizable, iview::ITouchable> AffiniteCalibrationShape;
typedef ibase::TMakeModelObserverCompWrap<icalibgui::CPerspectiveCalibrationShape, iview::IShape, imod::IObserver, iview::IVisualizable, iview::ITouchable> PerspectiveCalibrationShape;
typedef icalibgui::CPerspectiveCalibEditorComp PerspectiveCalibEditor;
typedef icalibgui::CSimpleLensCorrectionEditorComp SimpleLensCorrectionEditor;


} // namespace CalibrationGuiPck


#endif // !CalibrationGuiPck_included


