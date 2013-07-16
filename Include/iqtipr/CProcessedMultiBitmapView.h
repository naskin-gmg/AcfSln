#ifndef iqtipr_CProcessedMultiBitmapView_included
#define iqtipr_CProcessedMultiBitmapView_included


// Qt includes
#include <QtCore/QList>
#include <QtGui/QWidget>

// ACF includes
#include "iimg/IMultiBitmapProvider.h"


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
