#include "QtSignalPck.h"


// Qt includes
#include <QDir>


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


namespace QtSignalPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);


I_EXPORT_PACKAGE(
			"IacfQt",
			"Imaging tools Qt-based general package",
			"Imaging Tools Qt Standard" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			SamplingParamsGui,
			"Sampling parameters",
			"Interval Sample Sampling Model Parameters GUI Qt");

I_EXPORT_COMPONENT(
			SampleAcquisitionGui,
			"Sample acquisition GUI",
			"Sample Sampling Acquisition GUI");

I_EXPORT_COMPONENT(
			ScriptDataProcessor,
			"Test sample acquisition based on QtScript",
			"Test Sample Sampling Acquisition Script QtScript");


} // namespace QtSignalPck


