#include "QsciPck.h"


#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"


namespace QsciPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);


I_EXPORT_PACKAGE(
			"Qsci",
			"QScintilla package",
			"QScintilla Text" IM_TAG("Qt") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			ScintillaTextEditor,
			"Scintilla based text editor",
			"Qt Text Editor GUI Scintilla" IM_TAG("GUI Editor"));


} // namespace QsciPck


