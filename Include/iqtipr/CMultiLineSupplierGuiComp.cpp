#include "iqtipr/CMultiLineSupplierGuiComp.h"


// ACF includes
#include "iview/CPolylineShape.h"
#include "iprm/TParamsPtr.h"


namespace iqtipr
{


CMultiLineSupplierGuiComp::CMultiLineSupplierGuiComp()
{
}


// protected slots

void CMultiLineSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CMultiLineSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

void CMultiLineSupplierGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoUpdateButton->isChecked()){
		on_TestButton_clicked();
	}
}


QWidget* CMultiLineSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CMultiLineSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iinsp::ISupplier* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}
	if (objectPtr->GetWorkStatus() != iinsp::ISupplier::WS_OK){
		return;
	}

	CShape* shapePtr = new CShape(m_lineSelection);
	shapePtr->SetTransformMode(iview::CShapeBase::STM_SHAPE);

	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		if (modelPtr->IsAttached(shapePtr)){
			modelPtr->DetachObserver(shapePtr);
		}

		modelPtr->AttachObserver(shapePtr);
	}

	result.PushBack(shapePtr);
}


void CMultiLineSupplierGuiComp::OnGuiHidden()
{
	AutoUpdateButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// internal class CLineSelection

CMultiLineSupplierGuiComp::LineSelection::LineSelection()
:	m_selectedLineIndex(-1)
{

}


// reimplemented (iprm::ISelectionParam)

const iprm::IOptionsList* CMultiLineSupplierGuiComp::LineSelection::GetSelectionConstraints() const
{
	return NULL;
}


int CMultiLineSupplierGuiComp::LineSelection::GetSelectedOptionIndex() const
{
	return m_selectedLineIndex;
}


bool CMultiLineSupplierGuiComp::LineSelection::SetSelectedOptionIndex(int index)
{
	if (m_selectedLineIndex != index){
		istd::CChangeNotifier updatePtr(this);

		m_selectedLineIndex = index;
	}

	return true;
}


iprm::ISelectionParam* CMultiLineSupplierGuiComp::LineSelection::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CMultiLineSupplierGuiComp::LineSelection::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


// internal class CShape

CMultiLineSupplierGuiComp::CShape::CShape(iprm::ISelectionParam& selection)
	:m_lineSelection(selection)
{
	RegisterModel(dynamic_cast<imod::IModel*>(&m_lineSelection));
}


CMultiLineSupplierGuiComp::CShape::~CShape()
{
	EnsureModelDetached();

	UnregisterAllModels();
}


// reimplemented (iview::IVisualizable)

void CMultiLineSupplierGuiComp::CShape::Draw(QPainter& drawContext) const
{
	static QPen normalPen(QColor(10, 255, 116), 1, Qt::SolidLine, Qt::RoundCap);
	static QPen selectionPen(Qt::magenta, 3, Qt::SolidLine, Qt::RoundCap);

	imeas::INumericValueProvider* objectPtr = dynamic_cast<imeas::INumericValueProvider*>(GetModelPtr());
	Q_ASSERT(objectPtr != NULL);

	drawContext.setPen(normalPen);

	int linesCount = objectPtr->GetValuesCount();
	int selectedLineIndex = m_lineSelection.GetSelectedOptionIndex();

	for (int i = 0; i < linesCount; i++){
		const imath::CVarVector& lineValues = objectPtr->GetNumericValue(i).GetValues();
		Q_ASSERT(lineValues.GetElementsCount() == 4);

		i2d::CVector2d startPoint = GetScreenPosition(i2d::CVector2d(lineValues.GetElement(0), lineValues.GetElement(1)));
		i2d::CVector2d endPoint = GetScreenPosition(i2d::CVector2d(lineValues.GetElement(2), lineValues.GetElement(3)));

		if (selectedLineIndex == i){
			drawContext.setPen(selectionPen);
		}

		drawContext.drawLine(startPoint, endPoint);

		if (selectedLineIndex == i){
			drawContext.setPen(normalPen);
		}
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CMultiLineSupplierGuiComp::CShape::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Invalidate();
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CMultiLineSupplierGuiComp::CShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	imeas::INumericValueProvider* objectPtr = dynamic_cast<imeas::INumericValueProvider*>(GetModelPtr());
	if (objectPtr != NULL){
		const iview::IColorSchema& colorSchema = GetColorSchema();

		int linesCount = objectPtr->GetValuesCount();

		i2d::CRect boundingBox = i2d::CRect::GetEmpty();

		for (int i = 0; i < linesCount; i++){
			const imath::CVarVector& lineValues = objectPtr->GetNumericValue(i).GetValues();
			Q_ASSERT(lineValues.GetElementsCount() == 4);

			i2d::CVector2d startPoint = GetScreenPosition(i2d::CVector2d(lineValues.GetElement(0), lineValues.GetElement(1)));
			i2d::CVector2d endPoint = GetScreenPosition(i2d::CVector2d(lineValues.GetElement(2), lineValues.GetElement(3)));

			boundingBox.Union(startPoint.ToIndex2d());
			boundingBox.Union(endPoint.ToIndex2d());
		}

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_INACTIVE);

		return boundingBox.GetExpanded(tickerBox);
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iqtipr


