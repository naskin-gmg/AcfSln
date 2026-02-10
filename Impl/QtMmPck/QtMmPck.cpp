// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "QtMmPck.h"


#include <icomp/export.h>


namespace QtMmPck
{


using namespace icomp;


I_EXPORT_PACKAGE(
			"AcfSln/Multimedia/Gui",
			"Qt multimedia package",
			"Media" IM_PROJECT("ACF") IM_TAG("Qt Multimedia") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			VideoPlayerGui,
			"GUI used to create video player with parameters and control bar",
			"Qt Video File Loader Play" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			MediaLoader,
			"Loader for multimedia files",
			"Video File Loader Play" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			PlaybackControllerGui,
			"Simple playback controller",
			"Play Controller Frame Sequence Video File Loader Play" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			FrameSeqVideoController,
			"Video controller playing images from directory on external bitmaps",
			"Frame Sequence Bitmaps Images Video Medium Multimedia Directory Play" IM_TAG("Image"));

I_EXPORT_COMPONENT(
			SvgBitmapLoader,
			"Loader for vector graphics SVG files rendering it to bitmap",
			"Bitmap Vector Renderer SVG QT Scalable Graphics" IM_TAG("Image File Loader"));


} // namespace QtMmPck


