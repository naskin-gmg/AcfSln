#pragma once


// Qt includes
#include <QtCore/QEvent>


namespace iauth
{


class CLogoutEvent: public QEvent
{
public:
	CLogoutEvent();

	static QEvent::Type GenerateType();

private:
	static QEvent::Type m_type;
};


}	// namespace iauth


