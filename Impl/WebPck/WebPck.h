#ifndef WebPck_included
#define WebPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

// ACF-Solutions includes
#include <iweb/CWebServerStatus.h>


/**
	Web-service component package.
*/
namespace WebPck
{


typedef icomp::TModelCompWrap<icomp::TMakeComponentWrap<iweb::CWebServerStatus, iweb::IWebServerStatus>> WebServerStatus;


} // namespace WebPck


#endif // !WebPck_included


