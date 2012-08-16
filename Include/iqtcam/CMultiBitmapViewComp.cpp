#include "iqtcam/CMultiBitmapViewComp.h"


// ACF includes
#include <iview/CViewport.h>

// Qt includes
#include <QtGui/QGridLayout>


namespace iqtcam
{


// reimplemented (imod::CMultiModelDispatcherBase)

void CMultiBitmapViewComp::OnModelChanged(int modelId, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	// view index is equal to the modelId
	int index = modelId;

	if (!IsGuiCreated() || index < 0 || index >= m_views.count()){
		return;
	}

	CSingleView* viewPtr = m_views.at(index);
	viewPtr->SetInspectionResult(m_informationProvidersCompPtr[index]->GetInformationCategory());
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CMultiBitmapViewComp::UpdateGui(int updateFlags)
{
	for (int index = 0; index < m_views.count() && index < m_viewExtendersCompPtr.GetCount(); index++){
		iqt2d::IViewExtender* viewExtenderPtr = m_viewExtendersCompPtr[index];
		I_ASSERT(viewExtenderPtr != NULL);

		CSingleView* viewPtr = m_views.at(index);
		viewExtenderPtr->RemoveItemsFromScene(viewPtr);
		viewExtenderPtr->AddItemsToScene(viewPtr, updateFlags);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CMultiBitmapViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_columnCount = m_horizontalViewsAttrPtr.IsValid() ? qMax(1, *m_horizontalViewsAttrPtr) : 1;
	m_rowCount = m_verticalViewsAttrPtr.IsValid() ? qMax(1, *m_verticalViewsAttrPtr) : 1;

	int viewsCount = qMin(m_informationProvidersCompPtr.GetCount(), m_rowCount * m_columnCount);

	QWidget* widgetPtr = GetQtWidget();
	QGridLayout* layoutPtr = new QGridLayout(widgetPtr);
	layoutPtr->setContentsMargins(0, 0, 0, 0);
	widgetPtr->setLayout(layoutPtr);

	int viewIndex = 0;
	for (int row = 0; row < m_rowCount; row++){
		for (int col = 0; col < m_columnCount; col++){
			if (viewIndex >= viewsCount){
				break;
			}

			QString title = m_informationProvidersCompPtr[viewIndex]->GetInformationSource();
			CSingleView* viewPtr = CreateView(widgetPtr, viewIndex, title);
			layoutPtr->addWidget(viewPtr, row, col);
			m_views.append(viewPtr);

			imod::IModel* modelPtr = m_informationModelsCompPtr[viewIndex];
			I_ASSERT(modelPtr != NULL);
			bool isModelConnected = RegisterModel(modelPtr, viewIndex);
			I_ASSERT(isModelConnected);

			viewPtr->Init();

			viewIndex++;
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CMultiBitmapViewComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


void CMultiBitmapViewComp::OnComponentDestroyed()
{
	UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


// protected methods

CMultiBitmapViewComp::CSingleView* CMultiBitmapViewComp::CreateView(QWidget* parentPtr, int id, const QString& title)
{
	return new CSingleView(parentPtr, id, title);
}


// embedded class CView

CMultiBitmapViewComp::CSingleView::CSingleView(QWidget* parentPtr, int id, const QString& title)
:	BaseClass(parentPtr),
	m_console(this),
	m_id(id)
{
	setTitle(title);
}


void CMultiBitmapViewComp::CSingleView::Init()
{
	// create default layout
	QVBoxLayout* viewLayoutPtr = new QVBoxLayout;
	setLayout(viewLayoutPtr);

	// add console
	viewLayoutPtr->addWidget(&m_console);

	// add status label(s)
	AddStatusItems(viewLayoutPtr);
}


void CMultiBitmapViewComp::CSingleView::SetInspectionResult(int result)
{
	static QString s_defaultStyleSheet = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
		"stop: 0 #ffffff, stop: 0.5 #e0f0ff, stop: 0.51 #d0e0ee, stop: 1 #d0e0ee); "
		"border: 1px solid #aaaaaa; "
		"border-radius: 2px; "
		"color: #999999; font-size: 10pt; font-weight: bold;";

	static QString s_okStyleSheet = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
		"stop: 0.0 rgba(120,250,145), stop: 0.49 rgba(70,212,145), stop: 0.52 rgba(70,200,105), stop: 1.0 rgba(70,250,105)); "
		"border: 1px solid #339933; "
		"border-radius: 2px; "
		"color: #000000; font-size: 10pt; font-weight: bold;";

	static QString s_errorStyleSheet = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
		"stop: 0.0 rgba(250,120,145), stop: 0.49 rgba(212,120,145), stop: 0.52 rgba(200,70,105), stop: 1.0 rgba(250,120,145)); "
		"border: 1px solid #993333; "
		"border-radius: 2px; "
		"color: #ffffff; font-size: 10pt; font-weight: bold;";

	static QString s_warningStyleSheet = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
		"stop: 0.0 rgba(250,250,145), stop: 0.49 rgba(212,212,145), stop: 0.52 rgba(200,200,105), stop: 1.0 rgba(250,250,105)); "
		"border: 1px solid #996633; "
		"border-radius: 2px; "
		"color: #000000; font-size: 10pt; font-weight: bold;";

	switch (result){
		case istd::IInformationProvider::IC_INFO:
			m_statusLabel->setStyleSheet(s_okStyleSheet);
			return;

		case istd::IInformationProvider::IC_ERROR:
		case istd::IInformationProvider::IC_CRITICAL:
			m_statusLabel->setStyleSheet(s_errorStyleSheet);
			return;

		case istd::IInformationProvider::IC_WARNING:
			m_statusLabel->setStyleSheet(s_warningStyleSheet);
			return;
	}

	m_statusLabel->setStyleSheet(s_defaultStyleSheet);
}


// reimplemented (iqt2d::IViewProvider)

int CMultiBitmapViewComp::CSingleView::GetViewId() const
{
	return m_id;
}


iview::IShapeView* CMultiBitmapViewComp::CSingleView::GetView() const
{
	return &(m_console.GetViewRef());
}


// protected members

void CMultiBitmapViewComp::CSingleView::AddStatusItems(QBoxLayout* layoutPtr)
{
	// create default status label
	m_statusLabel = new QLabel(this);
	layoutPtr->addWidget(m_statusLabel);

	SetInspectionResult(istd::IInformationProvider::IC_NONE);
}


} // namespace iqtcam
