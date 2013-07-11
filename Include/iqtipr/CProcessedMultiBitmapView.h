#ifndef iqtipr_CProcessedMultiBitmapView_included
#define iqtipr_CProcessedMultiBitmapView_included


// Qt includes
#include <QtCore/QList>
#include <QtGui/QWidget>

// ACF includes
#include "iipr/IMultiBitmapProvider.h"


namespace iqtipr
{


class CProcessedBitmapView;


class CProcessedMultiBitmapView: public QWidget
{
public:
	CProcessedMultiBitmapView(QWidget* parent = NULL);
	
	void Update(iipr::IMultiBitmapProvider* bitmapProviderPtr);

private:
	QList<CProcessedBitmapView*> m_views;
};


} // namespace iqtipr


#endif // !iqtipr_CProcessedMultiBitmapView_included
