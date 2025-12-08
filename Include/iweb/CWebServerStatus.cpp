#include <iweb/CWebServerStatus.h>


// ACF inlcudes
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>


namespace iweb
{


CWebServerStatus::CWebServerStatus()
	:m_serverStatus(SS_NONE)
{
}


// reimplemented (iweb::IWebServerStatus)

IWebServerStatus::ServerStatus CWebServerStatus::GetServerStatus() const
{
	return m_serverStatus;
}


void CWebServerStatus::SetServerStatus(ServerStatus status)
{
	if (m_serverStatus != status){
		istd::CChangeNotifier changeNotifier(this);

		m_serverStatus = status;
	}
}


// reimplemented (istd::IChangeable)

int CWebServerStatus::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY | SO_RESET;
}


bool CWebServerStatus::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CWebServerStatus* sourcePtr = dynamic_cast<const CWebServerStatus*>(&object);
	if (sourcePtr != NULL){
		SetServerStatus(sourcePtr->GetServerStatus());

		return true;
	}

	return false;
}


bool CWebServerStatus::IsEqual(const IChangeable & object) const
{
	const CWebServerStatus* sourcePtr = dynamic_cast<const CWebServerStatus*>(&object);
	if (sourcePtr != NULL){
		return (m_serverStatus == sourcePtr->GetServerStatus());
	}

	return false;
}


istd::IChangeableUniquePtr CWebServerStatus::CloneMe(CompatibilityMode /*mode*/) const
{
	istd::TDelPtr<CWebServerStatus> retVal(new CWebServerStatus);
	if (retVal->CopyFrom(*this)){
		return retVal.PopPtr();
	}

	return NULL;
}


bool CWebServerStatus::ResetData(CompatibilityMode /*mode*/)
{
	m_serverStatus = SS_NONE;

	return true;
}


} // namespace iweb


