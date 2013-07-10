#include "iqtcam/CMultiBitmapViewComp.h"


// ACF includes
#include <iview/CViewport.h>

// Qt includes
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>


namespace iqtcam
{


// reimplemented (imod::IModelEditor)

void CMultiBitmapViewComp::UpdateEditor(int updateFlags)
{
	if (m_floatWindowPtr != NULL){
		if (!IsUpdateBlocked() && IsGuiCreated()){
			UpdateBlocker updateBlocker(this);

			UpdateGui(updateFlags);
		}
	} else {
		BaseClass::UpdateEditor(updateFlags);
	}
}


void CMultiBitmapViewComp::UpdateModel() const
{
	// do nothing here
}


// reimplemented (iqtgui::IDialog)

int CMultiBitmapViewComp::ExecuteDialog(IGuiObject* /*parentPtr*/)
{
	if (m_floatWindowPtr == NULL){
		return QDialog::Rejected;
	}

	m_floatWindowPtr->move(m_floatPos);
	m_floatWindowPtr->show();

	return QDialog::Accepted;
}


// protected methods

QIcon CMultiBitmapViewComp::GetCategoryIcon(istd::IInformationProvider::InformationCategory category)
{
	static QIcon defaultIcon(":/Icons/StateUnknown.svg");
	static QIcon infoIcon(":/Icons/StateOk.svg");
	static QIcon warningIcon(":/Icons/StateWarning.svg");
	static QIcon errorIcon(":/Icons/StateInvalid.svg");

	switch (category){
	case istd::IInformationProvider::IC_INFO:
		return infoIcon;

	case istd::IInformationProvider::IC_WARNING:
		return warningIcon;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorIcon;

	default:
		return defaultIcon;
	}
}


void CMultiBitmapViewComp::DoCreateGui(QWidget* widgetPtr)
{
	if (!m_views.isEmpty()){
		return;
	}

	iipr::IMultiBitmapProvider* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	int totalViewsCount = objectPtr->GetBitmapsCount();
	if (totalViewsCount == 0){
		return;
	}

	const iprm::IOptionsList* bitmapConstraintsPtr = objectPtr->GetBitmapSelectionContraints();

	QGridLayout* layoutPtr = new QGridLayout(widgetPtr);
	layoutPtr->setContentsMargins(0, 0, 0, 0);
	widgetPtr->setLayout(layoutPtr);

	if (m_horizontalViewsAttrPtr.IsValid() && m_verticalViewsAttrPtr.IsValid()){
		m_columnCount = qMax(1, *m_horizontalViewsAttrPtr);
		m_rowCount = qMax(1, *m_verticalViewsAttrPtr);
	} else if (m_horizontalViewsAttrPtr.IsValid()){
		m_columnCount = qMax(1, *m_horizontalViewsAttrPtr);
		m_rowCount = totalViewsCount / m_columnCount;
		if (totalViewsCount % m_columnCount){
			m_rowCount++;
		}
	} else if (m_verticalViewsAttrPtr.IsValid()){
		m_rowCount = qMax(1, *m_verticalViewsAttrPtr);
		m_columnCount = totalViewsCount / m_rowCount;
		if (totalViewsCount % m_rowCount){
			m_columnCount++;
		}
	} else {
		m_columnCount = qMax(1, (int)sqrt((double)totalViewsCount));
		m_rowCount = totalViewsCount / m_columnCount;
		if (totalViewsCount % m_columnCount){
			m_rowCount++;
		}
	}

	QColor backgroundColor = m_viewBackgroundColorAttrPtr.IsValid() ? 
		QColor(QString(*m_viewBackgroundColorAttrPtr)) : 
		Qt::transparent;

	int viewIndex = 0;
	for (int row = 0; row < m_rowCount; row++){
		for (int col = 0; col < m_columnCount; col++){
			QString title;

			if (m_viewLabelPrefixesAttrPtr.IsValid()){
				if (m_viewLabelPrefixesAttrPtr.GetCount() == 1){
					title = QString("%1 %2").arg(m_viewLabelPrefixesAttrPtr[0]).arg(viewIndex + 1);
				}
				else if (viewIndex < m_viewLabelPrefixesAttrPtr.GetCount()){
					title = m_viewLabelPrefixesAttrPtr[viewIndex];
				}
			}
			else if (viewIndex < m_informationProvidersCompPtr.GetCount()){
				title = m_informationProvidersCompPtr[viewIndex]->GetInformationSource();
			}
			else if (bitmapConstraintsPtr != NULL){
				if (viewIndex < bitmapConstraintsPtr->GetOptionsCount()){
					title = bitmapConstraintsPtr->GetOptionDescription(viewIndex);
					if (title.isEmpty()){
						title = bitmapConstraintsPtr->GetOptionName(viewIndex);
					}
				}
			}

			CSingleView* viewPtr = CreateView(widgetPtr, viewIndex, title);
			layoutPtr->addWidget(viewPtr, row, col);
			m_views.append(viewPtr);

			if (m_viewBackgroundColorAttrPtr.IsValid()){
				viewPtr->SetBackgroundColor(backgroundColor);
			}

			if (viewIndex < m_informationModelsCompPtr.GetCount()){
				imod::IModel* modelPtr = m_informationModelsCompPtr[viewIndex];
				Q_ASSERT(modelPtr != NULL);

				RegisterModel(modelPtr, viewIndex);
			}

			viewPtr->Init(*m_showStatusLabelAttrPtr, *m_showStatusBackgroundAttrPtr);

			viewIndex++;
		}
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CMultiBitmapViewComp::OnModelChanged(int modelId, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	// update general status if available
	if (modelId == GeneralStatusModelId){
		istd::IInformationProvider::InformationCategory generalResultCategory = m_generalInformationProviderCompPtr->GetInformationCategory();

		SetStatusIcon(GetCategoryIcon(generalResultCategory));

		return;
	}

	// view index is equal to the modelId
	int index = modelId;

	if (!IsGuiCreated() || index < 0 || index >= m_views.count()){
		return;
	}

	istd::IInformationProvider::InformationCategory viewResultCategory = m_informationProvidersCompPtr[index]->GetInformationCategory();

	CSingleView* viewPtr = m_views.at(index);
	viewPtr->SetInspectionResult(viewResultCategory);
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CMultiBitmapViewComp::UpdateGui(int updateFlags)
{
	QSize floatSize;
	if (m_floatWindowPtr != NULL){
		DoCreateGui(m_floatWindowPtr);

		floatSize = m_floatWindowPtr->size();
		m_floatPos = m_floatWindowPtr->pos();
	} else {
		DoCreateGui(GetQtWidget());
	}

	iipr::IMultiBitmapProvider* objectPtr = GetObjectPtr();

	int bitmapsCount = objectPtr->GetBitmapsCount();
	int viewsCount = m_views.count();

	for (int bitmapIndex = 0; bitmapIndex < bitmapsCount; bitmapIndex++){
		if (bitmapIndex < viewsCount){
			CSingleView* viewPtr = m_views.at(bitmapIndex);

			viewPtr->UpdateImage(objectPtr->GetBitmap(bitmapIndex));
		}
	}

	for (int index = 0; index < m_views.count() && index < m_viewExtendersCompPtr.GetCount(); index++){
		iqt2d::IViewExtender* viewExtenderPtr = m_viewExtendersCompPtr[index];
		Q_ASSERT(viewExtenderPtr != NULL);

		CSingleView* viewPtr = m_views.at(index);
		Q_ASSERT(viewPtr != NULL);

		viewExtenderPtr->RemoveItemsFromScene(viewPtr);
		viewExtenderPtr->AddItemsToScene(viewPtr, updateFlags);
	}

	if (m_floatWindowPtr != NULL){
		m_floatWindowPtr->resize(floatSize);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CMultiBitmapViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetQtWidget();

	if (*m_floatingWindowAttrPtr){
		m_floatWindowPtr = new QDialog(GetQtWidget());
		m_floatWindowPtr->setModal(false);
		m_floatWindowPtr->resize(800, 600);
		m_floatWindowPtr->setWindowTitle(tr("Images Preview"));
		m_floatWindowPtr->setWindowIcon(QIcon(":/Icons/Luppe"));
		m_floatWindowPtr->setSizeGripEnabled(true);

		QToolButton* showButtonPtr = new QToolButton(widgetPtr);
		showButtonPtr->setText(tr("Preview..."));
		widgetPtr->setLayout(new QHBoxLayout);
		widgetPtr->layout()->addWidget(showButtonPtr);

		connect(showButtonPtr, SIGNAL(clicked()), m_floatWindowPtr, SLOT(show()));
	}

	if (m_generalInformationModelCompPtr.IsValid() && m_generalInformationProviderCompPtr.IsValid()){
		RegisterModel(m_generalInformationModelCompPtr.GetPtr(), GeneralStatusModelId);
	}
}


// reimplemented (icomp::CComponentBase)

void CMultiBitmapViewComp::OnComponentCreated()
{
	m_floatWindowPtr = NULL;

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
	m_id(id),
	m_showStatusLabel(true),
	m_showStatusBackground(true)
{
	setTitle(title);

	m_backgroundShape.AssignToLayer(iview::IViewLayer::LT_BACKGROUND);
	m_backgroundModel.AttachObserver(&m_backgroundShape);

	m_console.GetViewRef().ConnectShape(&m_backgroundShape);

	m_console.SetFitMode(iview::CConsoleBase::FM_BOTH);
	m_console.SetZoomToFit(true);
}


void CMultiBitmapViewComp::CSingleView::SetBackgroundColor(const QColor& color)
{
	iview::CColorSchema* colorShemaPtr = new iview::CColorSchema;
	colorShemaPtr->Assign(m_console.GetViewRef().GetDefaultColorSchema());

	colorShemaPtr->SetBrush(iview::IColorSchema::SB_BACKGROUND, color);

	m_console.GetViewRef().SetDefaultColorSchema(colorShemaPtr, true);
}


void CMultiBitmapViewComp::CSingleView::UpdateImage(const iimg::IBitmap* bitmapPtr)
{
	if (bitmapPtr == NULL){
		m_backgroundModel.ResetImage();
	}
	else{
		m_backgroundModel.CopyFrom(*bitmapPtr);

		istd::CIndex2d imageSize = bitmapPtr->GetImageSize();

		m_console.GetViewRef().SetFitArea(i2d::CRectangle(0, 0, imageSize.GetX(), imageSize.GetY()));
	}
}


void CMultiBitmapViewComp::CSingleView::Init(bool hasStatusLabel, bool hasStatusBackground)
{
	m_showStatusLabel = hasStatusLabel;
	m_showStatusBackground = hasStatusBackground;

	// enable title info
	if (hasStatusLabel && title().isEmpty()){
		setTitle(" "); // a trick, otherwise if QGroupBox has no title text, then it will not be shown.
	}

	// create default layout
	QVBoxLayout* viewLayoutPtr = new QVBoxLayout;
	setLayout(viewLayoutPtr);

	// add console
	QWidget* holderPtr = new QWidget(this);
	holderPtr->setLayout(new QVBoxLayout());
	holderPtr->layout()->setContentsMargins(0,0,0,0);
	holderPtr->layout()->addWidget(&m_console);

	viewLayoutPtr->addWidget(holderPtr);

	// add status label(s)
	AddStatusItems(viewLayoutPtr);

	// default status
	SetInspectionResult(istd::IInformationProvider::IC_NONE);
}


void CMultiBitmapViewComp::CSingleView::SetInspectionResult(int result)
{
	if (m_showStatusBackground){
		switch (result){
			case istd::IInformationProvider::IC_INFO:
				SetBackgroundColor(Qt::darkGreen);
				break;

			case istd::IInformationProvider::IC_ERROR:
			case istd::IInformationProvider::IC_CRITICAL:
				SetBackgroundColor(Qt::darkRed);
				break;

			case istd::IInformationProvider::IC_WARNING:
				SetBackgroundColor(Qt::darkYellow);
				break;

			default:
				SetBackgroundColor(Qt::lightGray);
		}
	}


	if (m_showStatusLabel){
		static QString s_defaultStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.5, y2:1, stop:0 #d5eefc, stop:0.5 #d5eefc, stop:0.51 #b8e3f9, stop:1 #a5dbf7);"
			"}";

		static QString s_okStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
			"stop: 0.0 rgba(120,250,145), stop: 0.49 rgba(70,212,145), stop: 0.52 rgba(70,200,105), stop: 1.0 rgba(70,250,105)); "
			"border: 1px solid #339933; "
			"color: #000000; font-size: 10pt; font-weight: bold;"
			"}";

		static QString s_errorStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
			"stop: 0.0 rgba(250,120,145), stop: 0.49 rgba(212,120,145), stop: 0.52 rgba(200,70,105), stop: 1.0 rgba(250,120,145)); "
			"border: 1px solid #993333; "
			"color: #ffffff; font-size: 10pt; font-weight: bold;"
			"}";

		static QString s_warningStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
			"stop: 0.0 rgba(250,250,145), stop: 0.49 rgba(212,212,145), stop: 0.52 rgba(200,200,105), stop: 1.0 rgba(250,250,105)); "
			"border: 1px solid #996633; "
			"color: #000000; font-size: 10pt; font-weight: bold;"
			"}";

		switch (result){
			case istd::IInformationProvider::IC_INFO:
				setStyleSheet(s_okStyleSheet);
				break;

			case istd::IInformationProvider::IC_ERROR:
			case istd::IInformationProvider::IC_CRITICAL:
				setStyleSheet(s_errorStyleSheet);
				break;

			case istd::IInformationProvider::IC_WARNING:
				setStyleSheet(s_warningStyleSheet);
				break;

			default:
				setStyleSheet(s_defaultStyleSheet);
		}
	}
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

void CMultiBitmapViewComp::CSingleView::AddStatusItems(QBoxLayout* /*layoutPtr*/)
{
	// do nothing here, it is only for extension
}


} // namespace iqtcam


