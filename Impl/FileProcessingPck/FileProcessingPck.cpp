// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "FileProcessingPck.h"


#include <icomp/export.h>


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

I_EXPORT_COMPONENT(
			ExternalFileConverter,
			"General file copy or transform using loaders and any object",
			"File Converter External Application" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("File Converter"));

I_EXPORT_COMPONENT(
			SelectableFileConverter,
			"Selectable file converter",
			"File Converter" IM_CATEGORY(I_DATA_PROCESSING I_SERVICE));

I_EXPORT_COMPONENT(
			FileConvertOverProcessor,
			"File conversion over processor",
			"File Converter" IM_CATEGORY(I_DATA_PROCESSING I_SERVICE));

I_EXPORT_COMPONENT(
			CopyFilesTree,
			"Copy files from one directory to another one",
			"Copy Dir Filter Installation Tool" IM_TAG("File Processing"));

I_EXPORT_COMPONENT(
			ComposedCopyFiles,
			"Composed file copy",
			"Copy Dir Files Composed Set List Multiple" IM_TAG("File Processing"));

I_EXPORT_COMPONENT(
			ProcessingResultsReview,
			"Process files and saves results to a report file",
			"Processing Result Review File" IM_TAG("File Processing"));

I_EXPORT_COMPONENT(
			FileInfoCopy,
			"Copy file attaching some additional information like version and license",
			"Copy Move Attach License Version Tags" IM_TAG("File Processing"));

I_EXPORT_COMPONENT(
			FileCopyOverLoader,
			"General file copy or transform using loaders and any object",
			"Copy Compile Transform Saver" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("File Loader"));

I_EXPORT_COMPONENT(
			CopyApp,
			"Copy application transforming files",
			"Copy Compile Transform Application" IM_CATEGORY(I_DATA_PROCESSING I_APPLICATION) IM_TAG("File"));

I_EXPORT_COMPONENT(
			RenderedObjectFileLoader,
			"Loader of the rendered preview for an object",
			"File Loader Preview");

I_EXPORT_COMPONENT(
			BitmapPreviewGenerator,
			"Generator of the bitmap preview",
			"File Preview Generator Bitmap");


} // namespace FileProcessingPck


