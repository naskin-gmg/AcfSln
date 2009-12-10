#ifndef iwin_CDefaultServicesProvider_included
#define iwin_CDefaultServicesProvider_included


#include "iwin/iwin.h"


namespace iwin
{


/**
	Default services provider for Windows plattform

	\ingroup System
*/
class CDefaultServicesProvider
{
public:
	static void RegisterServices();
};


} // namespace iwin


#endif // !iwin_CDefaultServicesProvider_included


