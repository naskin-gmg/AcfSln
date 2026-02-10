// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "QtMeasPck.h"

#include <icomp/export.h>

#include <iqtgui/iqtgui.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace QtMeasPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Measure/GUI",
			"Qt dependent package for sampling, measurement acquisition and related thema",
			"Measurement Acquisition GUI" IM_TAG("Qt Sampling") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DataSequenceView,
			"Display sample sequence",
			"Diagram Polyline Samples Sequence Measurement GUI" IM_TAG("2D GUI"));

I_EXPORT_COMPONENT(
			DataStatisticsGui,
			"Data statistics GUI",
			"Statistics Histogram Data" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			NumericParamsGui,
			"Editor for numeric parameters",
			"Numeric Values Filter Ranges Parameter Editor");

I_EXPORT_COMPONENT(
			DataSequenceSupplierResultsView,
			"Results view for any data sequence supplier/provider",
			"Task Supplier Observer Subtask Data Sequence Preview" IM_TAG("Inspection GUI Editor"));


} // namespace QtMeasPck


