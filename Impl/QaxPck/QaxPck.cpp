#include "QaxPck.h"


#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"


namespace QaxPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"Qax",
			"QActiveX based package",
			"ActiveX QActiveX COM OLE" IM_TAG("Qt Windows") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			VlcVideoViewGui,
			"Video widget based on VLC ActiveX",
			"VLC Qt Widget GUI Video Media Multimedia Play" IM_TAG("GUI Multimedia"));


} // namespace QaxPck


