#include <iblobgui/CBlobFeaturesSupplierGuiComp.h>


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include <imath/CVarVector.h>
#include <iipr/IFeaturesProvider.h>
#include <iblob/CBlobFeature.h>


namespace iblobgui
{


CBlobFeaturesSupplierGuiComp::CBlobFeaturesSupplierGuiComp()
:	m_lastViewPtr(NULL)
{
	connect(this, SIGNAL(OnSupplierParamsChangedSignal()), SLOT(on_TestButton_clicked()), Qt::QueuedConnection);
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CBlobFeaturesSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


void CBlobFeaturesSupplierGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoTestButton->isChecked()){
		Q_EMIT OnSupplierParamsChangedSignal();
	}
}


// reimplemented (iqt2d::IViewExtender)

void CBlobFeaturesSupplierGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	iview::IShapeView* viewPtr = providerPtr->GetView();
	if (viewPtr != NULL){
		m_lastViewPtr = viewPtr;

		ConnectShapes(*viewPtr);

		if (auto observerPtr = dynamic_cast<iview::CViewBase*>(viewPtr)) {
			observerPtr->AddViewEventObserver(this);
		}
	}
	
	BaseClass::AddItemsToScene(providerPtr, flags);
}


void CBlobFeaturesSupplierGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	iview::IShapeView* viewPtr = providerPtr->GetView();
	if (viewPtr != NULL){
		if (auto observerPtr = dynamic_cast<iview::CViewBase*>(viewPtr)) {
			observerPtr->RemoveViewEventObserver(this);
		}

		DisconnectShapes(*viewPtr);
	}

	m_lastViewPtr = NULL;

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// reimplemented (iview::IViewEventObserver)

bool CBlobFeaturesSupplierGuiComp::OnSelectChange(
			const iview::IShapeView& /*view*/,
			const istd::CIndex2d& /*position*/,
			const iview::IInteractiveShape& shape,
			bool state)
{
	for (auto it = m_shapeToItemMap.begin(); it != m_shapeToItemMap.end(); ++it) {
		auto shapePtr = &it.key();
		if (&shape == *shapePtr) {
			if (state)
				ResultsList->setCurrentItem(it.value());

			it.value()->setSelected(state);

			return true;
		}
	}

	return false;
}


bool CBlobFeaturesSupplierGuiComp::OnViewMouseButton(
			const iview::IShapeView& /*view*/,
			const istd::CIndex2d& /*position*/,
			Qt::MouseButton /*buttonType*/,
			bool /*state*/,
			const iview::IInteractiveShape* /*shapePtr*/)
{
	return false;
}


bool CBlobFeaturesSupplierGuiComp::OnViewMouseMove(
	const iview::IShapeView& /*view*/,
	const istd::CIndex2d& /*position*/)
{
	return false;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CBlobFeaturesSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());

	if (m_paramsBoxHeightAttrPtr.IsValid()) {
		ParamsGB->setMaximumHeight(*m_paramsBoxHeightAttrPtr);
	}
}


void CBlobFeaturesSupplierGuiComp::OnGuiModelDetached()
{
	ClearResults();

	BaseClass::OnGuiModelDetached();
}


void CBlobFeaturesSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	if (!changeSet.Contains(iinsp::ISupplier::CF_SUPPLIER_RESULTS)){
		return;
	}

	ClearResults();

	const QString mm = " " + QObject::tr("mm");
	const QString mm2 = mm + QCHAR_POW2;

	iinsp::ISupplier* supplierPtr = GetObservedObject();
	if (supplierPtr != NULL){
		iipr::IFeaturesProvider* providerPtr = CompCastPtr<iipr::IFeaturesProvider>(supplierPtr);
		if (providerPtr != NULL){
			int featuresCount = providerPtr->GetFeaturesCount();

			for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
				QTreeWidgetItem* modelItemPtr = new QTreeWidgetItem;

				const iblob::CBlobFeature& blobFeature = dynamic_cast<const iblob::CBlobFeature&>(providerPtr->GetFeature(featureIndex));
				auto calibrationPtr = blobFeature.GetCalibration();

				double diameter = blobFeature.GetWeight();

				// Text on mouse over
				QString description = blobFeature.GetObjectId();

				if (calibrationPtr) {	// show mm
					modelItemPtr->setText(CT_AREA, QString::number(blobFeature.GetArea(), 'f', 2) + mm2);
					modelItemPtr->setText(CT_PERIMETER, QString::number(blobFeature.GetPerimeter(), 'f', 2) + mm);
					modelItemPtr->setText(CT_DIAMETER, QString::number(diameter, 'f', 2) + mm);
					modelItemPtr->setText(CT_POSITION,
						QString::number(blobFeature.GetPosition().GetX(), 'f', 2) + "," +
						QString::number(blobFeature.GetPosition().GetY(), 'f', 2) + mm);

					description += tr(" at %1:%2 mm, Area %3%4, Diameter %5 mm")
						.arg(blobFeature.GetPosition().GetX(), 0, 'f', 2)
						.arg(blobFeature.GetPosition().GetY(), 0, 'f', 2)
						.arg(blobFeature.GetArea(), 0, 'f', 2)
						.arg(mm2)
						.arg(diameter, 0, 'f', 2);
				}
				else {	// show pixel
					modelItemPtr->setText(CT_AREA, QString::number(blobFeature.GetArea(), 'f', 2));
					modelItemPtr->setText(CT_PERIMETER, QString::number(blobFeature.GetPerimeter(), 'f', 2) );
					modelItemPtr->setText(CT_DIAMETER, QString::number(diameter, 'f', 2));
					modelItemPtr->setText(CT_POSITION,
						QString::number(blobFeature.GetPosition().GetX(), 'f', 2) + "," +
						QString::number(blobFeature.GetPosition().GetY(), 'f', 2));
				}

				modelItemPtr->setText(CT_ID, blobFeature.GetObjectId());
				modelItemPtr->setText(CT_CIRCULARITY, QString::number(blobFeature.GetCircularity(), 'f', 2));
				modelItemPtr->setText(CT_ANGLE, QString::number(imath::GetDegreeFromRadian(blobFeature.GetAngle()), 'f', 2) + QCHAR_DEGREE);

				// Add general model shape:
				if (*m_showResultShapesAttrPtr){
					auto visualObject = new VisualObject(false);
					if (!visualObject->model->CopyFrom(blobFeature.GetContour(), istd::IChangeable::CM_CONVERT)){
						visualObject->model->SetBaseObject(blobFeature.GetContour());
					}
					visualObject->SetAreaTouchAllowed(true);
					visualObject->SetDefaultDescription(description);

					visualObject->SetEditableNodes(false);
					visualObject->SetEditablePosition(false);
					visualObject->SetEditableRotation(false);

					QString toolTip = QString("<h2>%1</h2>").arg(QString(blobFeature.GetObjectId()));
					toolTip += "<hr>";
					toolTip += tr("<b>Area:</b> %1%2").arg(blobFeature.GetArea(), 0, 'f', 2).arg(mm2)
						+ tr("<p><b>Diameter:</b> %1 mm").arg(diameter, 0, 'f', 2)
						+ tr("<p><b>Perimeter:</b> %1 mm").arg(blobFeature.GetPerimeter(), 0, 'f', 2)
						+ tr("<p><b>Circularity:</b> %1").arg(blobFeature.GetCircularity(), 0, 'f', 2);
					visualObject->SetToolTip(toolTip);

					m_visualPositions.PushBack(visualObject);

					m_shapeToItemMap[visualObject] = modelItemPtr;
				}
					
				ResultsList->addTopLevelItem(modelItemPtr);
			}
		}

		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(supplierPtr->GetModelParametersSet());
		if (paramsModelPtr != NULL){
			if (!paramsModelPtr->IsAttached(&m_paramsObserver)){
				m_paramsObserver.EnsureModelDetached();

				paramsModelPtr->AttachObserver(&m_paramsObserver);
			}
		}
	}

	if (m_lastViewPtr != NULL){
		ConnectShapes(*m_lastViewPtr);
	}
}


// reimplemented (iqtgui::IGuiObject)

void CBlobFeaturesSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ResultsList->header()->setSectionResizeMode(QHeaderView::Interactive);
	ResultsList->header()->setMinimumSectionSize(100);
	ResultsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->CreateGui(IntermediateWidget);
	}
}


void CBlobFeaturesSupplierGuiComp::OnGuiHidden()
{
	AutoTestButton->setChecked(false);

	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiHidden();
}


void CBlobFeaturesSupplierGuiComp::OnGuiDesignChanged()
{
	BaseClass::OnGuiDesignChanged();

	AutoTestButton->setIcon(GetIcon(":/Icons/AutoUpdate"));
	TestButton->setIcon(GetIcon(":/Icons/Play"));
}


// reimplemented (icomp::IComponentBase)

void CBlobFeaturesSupplierGuiComp::OnComponentDestroyed()
{
	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// protected slots

void CBlobFeaturesSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CBlobFeaturesSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CBlobFeaturesSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


void CBlobFeaturesSupplierGuiComp::on_ResultsList_itemSelectionChanged()
{
	for (auto it = m_shapeToItemMap.begin(); it != m_shapeToItemMap.end(); ++it) {
		auto shapePtr = it.key();
		auto itemPtr = it.value();
		shapePtr->SetSelected(itemPtr->isSelected());
	}
}


// private methods

void CBlobFeaturesSupplierGuiComp::ConnectShapes(iview::IShapeView& view)
{
	int shapesCount = m_visualPositions.GetCount();
	for (int shapeIndex = 0; shapeIndex < shapesCount; shapeIndex++){
		iview::IShape* shapePtr = m_visualPositions.GetAt(shapeIndex);
		Q_ASSERT(shapePtr != NULL);

		view.ConnectShape(shapePtr);
	}
}


void CBlobFeaturesSupplierGuiComp::DisconnectShapes(iview::IShapeView& view)
{
	int shapesCount = m_visualPositions.GetCount();
	for (int shapeIndex = 0; shapeIndex < shapesCount; shapeIndex++){
		iview::IShape* shapePtr = m_visualPositions.GetAt(shapeIndex);
		Q_ASSERT(shapePtr != NULL);

		view.DisconnectShape(shapePtr);
	}
}


void CBlobFeaturesSupplierGuiComp::ClearResults()
{
	ResultsList->clear();

	if (m_lastViewPtr != NULL){
		DisconnectShapes(*m_lastViewPtr);
	}

	m_visualPositions.Reset();
	m_shapeToItemMap.clear();
}


} // namespace iblobgui


