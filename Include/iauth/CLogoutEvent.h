#pragma once


// Qt includes
#include <QtCore/QEvent>


namespace iauth
{


class CLogoutEvent: public QEvent
{
public:
	CLogoutEvent();

	bool IsLogoutCanceled();
	void CancelLogout();

	static QEvent::Type type();

private:
	static QEvent::Type GenerateType();

private:
	bool m_isCanceled;

	static QEvent::Type m_type;
};


}	// namespace iauth


