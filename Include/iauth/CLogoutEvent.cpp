#include <iauth/CLogoutEvent.h>"


namespace iauth
{


// public methods

CLogoutEvent::CLogoutEvent()
	:QEvent(CLogoutEvent::GenerateType())
{
}


QEvent::Type CLogoutEvent::GenerateType()
{
	if (m_type == QEvent::None){
		m_type = static_cast<QEvent::Type>(QEvent::registerEventType());
	}

	return m_type;
}


QEvent::Type CLogoutEvent::m_type = QEvent::None;


}	// namespace iauth


