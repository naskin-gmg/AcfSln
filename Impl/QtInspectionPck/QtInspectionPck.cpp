#include "QtInspectionPck.h"


// ACF includes
#include "icomp/export.h"


I_REGISTER_QT_RESOURCE(iqtgui);


namespace QtInspectionPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Inspection/Gui",
			"General Qt-based image processing package",
			"Qt Processing Image" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			InspectionTaskGui,
			"GUI for general inspection task",
			"Task Supplier Composite Observer" IM_TAG("Inspection GUI Editor"));

I_EXPORT_COMPONENT(
			GeneralSupplierGui,
			"General GUI for supplier (usable as subtask GUI)",
			"Task Supplier Observer Subtask" IM_TAG("Inspection GUI Editor"));


} // namespace QtInspectionPck


