// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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


