// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtipr_CProcessedMultiBitmapView_included
#define iqtipr_CProcessedMultiBitmapView_included


// Qt includes
#include <QtCore/QList>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <iimg/IMultiBitmapProvider.h>


namespace iqtipr
{


class CProcessedBitmapView;


class CProcessedMultiBitmapView: public QWidget
{
public:
	CProcessedMultiBitmapView(QWidget* parent = NULL);
	
	void Update(const iimg::IMultiBitmapProvider* bitmapProviderPtr);

private:
	QList<CProcessedBitmapView*> m_views;
};


} // namespace iqtipr


#endif // !iqtipr_CProcessedMultiBitmapView_included
