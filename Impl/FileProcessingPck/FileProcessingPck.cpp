#include "FileProcessingPck.h"


#include "icomp/export.h"


namespace FileProcessingPck
{


I_EXPORT_PACKAGE(
			"AcfSln/FileProcessing",
			"File Processing package",
			"File" IM_TAG("Processing") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FileNaming,
			"Component for file (re)naming",
			"File Renaming Directory");

I_EXPORT_COMPONENT(
			FileNamingParams,
			"Parmeter for file (re)naming",
			"File Processing Hotfolder Parameter");

I_EXPORT_COMPONENT(
			FileListConverter,
			"Component for conversion of a list of files",
			"File Processing Conversion List");

I_EXPORT_COMPONENT(
			FilesContainer,
			"A simple container of file pathes",
			"File List Container");


} // namespace FileProcessingPck


