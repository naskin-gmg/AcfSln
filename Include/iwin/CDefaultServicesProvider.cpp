#include "iwin/CDefaultServicesProvider.h"


#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/CSimpleDateTime.h"

#include "iwin/CFileSystem.h"
#include "iwin/CTimer.h"
#include "iwin/CProcessEnvironment.h"
#include "iwin/CCriticalSection.h"
#include "iwin/CSystemEnvironment.h"
#include "iwin/CSystemInfo.h"


namespace iwin
{


void CDefaultServicesProvider::RegisterServices()
{
	static iwin::CProcessEnvironment applicationEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::IProcessEnvironment>(&applicationEnvironment);

	static iwin::CCriticalSection criticalSection;
	istd::CStaticServicesProvider::RegisterService<isys::ICriticalSection>(&criticalSection);

	static iwin::CFileSystem fileSystem;
	istd::CStaticServicesProvider::RegisterService<isys::IFileSystem>(&fileSystem);

	static iwin::CSystemEnvironment systemEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::ISystemEnvironment>(&systemEnvironment);

	static istd::TSingleFactory<isys::ITimer, iwin::CTimer> timerFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&timerFactory);

	static istd::TSingleFactory<isys::IDateTime, isys::CSimpleDateTime> dateTimeFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&dateTimeFactory);

	static istd::TSingleFactory<isys::ISystemInfo, iwin::CSystemInfo> systemInfoFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&systemInfoFactory);
}


} // namespace iwin


