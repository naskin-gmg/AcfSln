#include "iqtipr/CProcessedMultiBitmapView.h"


// Qt includes
#include <QtGui/QVBoxLayout>


// ACF includes
#include "iqtipr/CProcessedBitmapView.h"


namespace iqtipr
{


CProcessedMultiBitmapView::CProcessedMultiBitmapView(QWidget* parent): QWidget(parent)
{
	setLayout(new QVBoxLayout());

	// default size
	resize(800,600);
}


void CProcessedMultiBitmapView::Update(iipr::IMultiBitmapProvider* bitmapProviderPtr)
{
	for (int i = 0; i < m_views.count(); i++){
		m_views.at(i)->hide();
	}

	if (bitmapProviderPtr == NULL){
		return;
	}

	int bitmapCount = bitmapProviderPtr->GetBitmapsCount();
	while (m_views.count() < bitmapCount){
		CProcessedBitmapView* viewPtr = new CProcessedBitmapView(this);
		layout()->addWidget(viewPtr);
		m_views.append(viewPtr);
	}

	const iprm::IOptionsList* constraintsPtr = bitmapProviderPtr->GetBitmapSelectionContraints();

	for (int i = 0; i < bitmapCount; i++){
		CProcessedBitmapView* viewPtr = m_views.at(i);

		viewPtr->SetBitmap(bitmapProviderPtr->GetBitmap(i));

		if (constraintsPtr != NULL){
			viewPtr->SetTitle(constraintsPtr->GetOptionName(i));
		}

		viewPtr->show();
	}
}


} // namespace iqtipr
