#include "QtIprPck.h"


// Qt includes
#include <QtCore/QDir>


// ACF includes
#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"


namespace QtIprPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"QtIpr",
			"Standard Qt image processing package",
			"Qt \"Image Processing\"" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			SearchGui,
			"SearchGui",
			"Model Search Qt Image Processing");

I_EXPORT_COMPONENT(
		SearchParamsGui,
		"SearchParamsGui",
		"GUI Model Search Parameter Qt Image Processing");

I_EXPORT_COMPONENT(
			ValueSupplierGui,
			"GUI for supplier implementation returning position found using caliper tool",
			"GUI Observer Caliper Position Qt Processing Supplier");

I_EXPORT_COMPONENT(
			CaliperParamsGui,
			"GUI to parametrize caliper",
			"GUI Observer Caliper Parameters Direction Edge Mode Threshold Qt");

I_EXPORT_COMPONENT(
			LineProjectionSupplierGui,
			"GUI for line projection supplier",
			"GUI Observer Supplier Line Projection Image Processing Qt");

I_EXPORT_COMPONENT(
			CircleFinderParamsGui,
			"GUI to parametrize the circle finder",
			"GUI Observer Caliper Circle Parameters Qt");


} // namespace QtIprPck


