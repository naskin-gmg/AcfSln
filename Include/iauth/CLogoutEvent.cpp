#include <iauth/CLogoutEvent.h>


namespace iauth
{


// public methods

CLogoutEvent::CLogoutEvent()
	:QEvent(CLogoutEvent::GenerateType()),
	m_isCanceled(false)
{
}


bool CLogoutEvent::IsLogoutCanceled()
{
	return m_isCanceled;
}


void CLogoutEvent::CancelLogout()
{
	m_isCanceled = true;
}


// public static methods

QEvent::Type CLogoutEvent::type()
{
	return m_type;
}


// private static methods

QEvent::Type CLogoutEvent::GenerateType()
{
	if (m_type == QEvent::None){
		m_type = static_cast<QEvent::Type>(QEvent::registerEventType());
	}

	return m_type;
}


QEvent::Type CLogoutEvent::m_type = QEvent::None;


}	// namespace iauth


