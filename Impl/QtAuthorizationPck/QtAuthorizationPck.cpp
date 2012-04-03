#include "QtAuthorizationPck.h"


// Qt includes
#include <QtCore/QDir>

// ACF includes
#include "icomp/export.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtAuthorizationPck
{


I_EXPORT_PACKAGE(
			"AcfSln/QtAuthorizationPck",
			"GUI components for authorization",
			"GUI Qt" IM_PROJECT("ACF-Solutions") IM_TAG("Authorization") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			LoginBarGui,
			"Login GUI",
			"Login");

I_EXPORT_COMPONENT(
			UserGui,
			"GUI displaying list of users",
			"User Rights");


} // namespace QtAuthorizationPck


