#include "iqtipr/CValueSupplierGuiComp.h"

// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include "imath/CVarVector.h"
#include "iview/CCircleShape.h"
#include "iview/CPinShape.h"
#include "iview/CLineShape.h"


namespace iqtipr
{


CValueSupplierGuiComp::CValueSupplierGuiComp()
:	m_isCircleActive(false),
	m_isLineActive(false)
{
}


// protected slots

void CValueSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CValueSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CValueSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CValueSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


void CValueSupplierGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoUpdateButton->isChecked()){
		QTimer::singleShot(1, this, SLOT(on_TestButton_clicked()));
	}
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CValueSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	m_foundCircleModel.DetachAllObservers();
	m_foundLineModel.DetachAllObservers();

	if (m_isLineActive) {
		iview::CLineShape* lineShapePtr = new iview::CLineShape();
		if (lineShapePtr != NULL) {
			lineShapePtr->SetVisible(false);
			lineShapePtr->SetEditablePosition(false);

			m_foundLineModel.AttachObserver(lineShapePtr);

			result.PushBack(lineShapePtr);
		}
	}

	if (m_isCircleActive) {
		iview::CCircleShape* circleShapePtr = new iview::CCircleShape();
		if (circleShapePtr != NULL){
			circleShapePtr->SetVisible(false);
			circleShapePtr->SetEditablePosition(false);
			circleShapePtr->SetEditableRadius(false);

			m_foundCircleModel.AttachObserver(circleShapePtr);

			result.PushBack(circleShapePtr);
		}

		iview::CPinShape* posShapePtr = new iview::CPinShape();
		if (posShapePtr != NULL){
			posShapePtr->SetVisible(false);
			posShapePtr->SetEditablePosition(false);

			m_foundCircleModel.AttachObserver(posShapePtr);

			result.PushBack(posShapePtr);
		}
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CValueSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CValueSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	imath::CVarVector position;
	imath::CVarVector radius;
	imath::CVarVector line;

	iinsp::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		imeas::INumericValueProvider* providerPtr = dynamic_cast<imeas::INumericValueProvider*>(supplierPtr);
		if (providerPtr != NULL && providerPtr->GetValuesCount() > 0){
			const imeas::INumericValue& resultValue = providerPtr->GetNumericValue(0);

			position = resultValue.GetComponentValue(imeas::INumericValue::VTI_POSITION);
			radius = resultValue.GetComponentValue(imeas::INumericValue::VTI_RADIUS);
			line = resultValue.GetComponentValue(imeas::INumericValue::VTI_2D_LINE);
		}

		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(supplierPtr->GetModelParametersSet());
		if (paramsModelPtr != NULL){
			if (!paramsModelPtr->IsAttached(&m_paramsObserver)){
				m_paramsObserver.EnsureModelDetached();

				paramsModelPtr->AttachObserver(&m_paramsObserver);
			}
		}
	}

	bool isResultVisible = false;
	m_isCircleActive = false;
	m_isLineActive = false;

	if (line.GetElementsCount() >= 4){
		m_isLineActive = true;
		m_foundCircleModel.ResetData();
		m_foundLineModel.SetPoint1(i2d::CVector2d(line.GetElement(0),line.GetElement(1)));
		m_foundLineModel.SetPoint2(i2d::CVector2d(line.GetElement(2),line.GetElement(3)));

		isResultVisible = true;

		if (IsGuiCreated()){
			PositionLabel->setText(tr("(%1, %2)(%3, %4)").arg(line.GetElement(0)).arg(line.GetElement(1)).arg(line.GetElement(2)).arg(line.GetElement(3)));
		}
	}
	else{
		m_isCircleActive = true;
		m_foundLineModel.ResetData();
		if (position.GetElementsCount() >= 2){
			m_foundCircleModel.SetPosition(i2d::CVector2d(position[0], position[1]));

			isResultVisible = true;

			if (IsGuiCreated()){
				PositionLabel->setText(tr("(%1, %2)").arg(position[0]).arg(position[1]));
			}
		}
		else{
			m_foundCircleModel.SetPosition(i2d::CVector2d(0, 0));

			if (IsGuiCreated()){
				PositionLabel->setText(tr("No position"));
			}
		}

		if (radius.GetElementsCount() >= 1){
			m_foundCircleModel.SetRadius(radius[0]);
		}
		else{
			m_foundCircleModel.SetRadius(0);
		}
	}

	FoundCircleModel::Observers circleObservers = m_foundCircleModel.GetObservers();
	for (		FoundCircleModel::Observers::ConstIterator observerIter = circleObservers.constBegin();
				observerIter != circleObservers.constEnd();
				++observerIter){
		iview::CShapeBase* shapePtr = dynamic_cast<iview::CShapeBase*>(*observerIter);
		if (shapePtr != NULL){
			shapePtr->SetVisible(isResultVisible);
		}
	}

	FoundLineModel::Observers lineObservers = m_foundLineModel.GetObservers();
	for (		FoundLineModel::Observers::ConstIterator observerIter = lineObservers.constBegin();
				observerIter != lineObservers.constEnd();
				++observerIter){
		iview::CShapeBase* shapePtr = dynamic_cast<iview::CShapeBase*>(*observerIter);
		if (shapePtr != NULL){
			shapePtr->SetVisible(isResultVisible);
		}
	}

	UpdateAllViews();
}


// reimplemented (iqtgui::IGuiObject)

void CValueSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->CreateGui(IntResultsFrame);

		IntResultsFrame->setVisible(true);
	}
	else{
		IntResultsFrame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		IntResultsFrame->hide();
	}

	ParamsGB->adjustSize();
	IntResultsFrame->adjustSize();
}


void CValueSupplierGuiComp::OnGuiDestroyed()
{
	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->DestroyGui();
	}
	
	BaseClass::OnGuiDestroyed();
}


void CValueSupplierGuiComp::OnGuiHidden()
{
	AutoUpdateButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// reimplemented (icomp::IComponentBase)

void CValueSupplierGuiComp::OnComponentDestroyed()
{
	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqtipr


