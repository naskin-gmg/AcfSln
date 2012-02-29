#include "QtInspectionPck.h"


// Qt includes
#include <QtCore/QDir>


// ACF includes
#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtInspectionPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);


I_EXPORT_PACKAGE(
			"QtInspectionPck",
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


