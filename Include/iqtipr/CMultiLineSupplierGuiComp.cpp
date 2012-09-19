#include "iqtipr/CMultiLineSupplierGuiComp.h"


// ACF includes
#include "iview/CInteractivePolylineShape.h"
#include "iprm/TParamsPtr.h"
#include "iproc/IElapsedTimeProvider.h"


namespace iqtipr
{


CMultiLineSupplierGuiComp::CMultiLineSupplierGuiComp()
	:m_selectedLineIndex(-1)
{
}


// protected slots

void CMultiLineSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();

	iproc::IElapsedTimeProvider* timePtr = dynamic_cast<iproc::IElapsedTimeProvider*>(GetObjectPtr());
	if (timePtr != NULL){
		TimeLabel->setText(QString("%1 ms").arg(timePtr->GetElapsedTime() * 1000.0, 0, 'f', 2));
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CMultiLineSupplierGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	imeas::INumericValueProvider* objectPtr = dynamic_cast<imeas::INumericValueProvider*>(GetObjectPtr());
	I_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier changePtr(&m_results);

	m_results.CopyFrom(*objectPtr);
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
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CMultiLineSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	CShape* shapePtr = new CShape(*this);

	m_results.AttachObserver(shapePtr);

	result.PushBack(shapePtr);
}


// reimplemented (iprm::ISelectionParam)

const iprm::ISelectionConstraints* CMultiLineSupplierGuiComp::GetSelectionConstraints() const
{
	return NULL;
}


int CMultiLineSupplierGuiComp::GetSelectedOptionIndex() const
{
	return m_selectedLineIndex;
}


bool CMultiLineSupplierGuiComp::SetSelectedOptionIndex(int index)
{
	m_selectedLineIndex = index;

	return true;
}


iprm::ISelectionParam* CMultiLineSupplierGuiComp::GetActiveSubselection() const
{
	return NULL;
}


// internal class CShape

CMultiLineSupplierGuiComp::CShape::CShape(iprm::ISelectionParam& selection)
	:m_lineSelection(selection)
{
}


// reimplemented (iview::IVisualizable)

void CMultiLineSupplierGuiComp::CShape::Draw(QPainter& drawContext) const
{
	static QPen normalPen(QColor(10, 255, 116), 1, Qt::SolidLine, Qt::RoundCap);
	static QPen selectionPen(Qt::magenta, 3, Qt::SolidLine, Qt::RoundCap);

	imeas::INumericValueProvider* objectPtr = dynamic_cast<imeas::INumericValueProvider*>(GetModelPtr());
	I_ASSERT(objectPtr != NULL);

	drawContext.setPen(normalPen);

	int linesCount = objectPtr->GetValuesCount();
	int selectedLineIndex = m_lineSelection.GetSelectedOptionIndex();

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	for (int i = 0; i < linesCount; i++){
		const imath::CVarVector& lineValues = objectPtr->GetNumericValue(i).GetValues();
		I_ASSERT(lineValues.GetElementsCount() == 4);

		QPointF startPoint = transform.GetApply(i2d::CVector2d(lineValues.GetElement(0), lineValues.GetElement(1)));
		QPointF endPoint = transform.GetApply(i2d::CVector2d(lineValues.GetElement(2), lineValues.GetElement(3)));

		if (selectedLineIndex == i){
			drawContext.setPen(selectionPen);
		}

		drawContext.drawLine(startPoint, endPoint);

		if (selectedLineIndex == i){
			drawContext.setPen(normalPen);
		}
	}
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CMultiLineSupplierGuiComp::CShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	imeas::INumericValueProvider* objectPtr = dynamic_cast<imeas::INumericValueProvider*>(GetModelPtr());
	if (objectPtr != NULL){
		const iview::IColorShema& colorShema = GetColorShema();

		int linesCount = objectPtr->GetValuesCount();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		i2d::CRect boundingBox = i2d::CRect::GetEmpty();

		for (int i = 0; i < linesCount; i++){
			const imath::CVarVector& lineValues = objectPtr->GetNumericValue(i).GetValues();
			I_ASSERT(lineValues.GetElementsCount() == 4);

			istd::CIndex2d startPoint = transform.GetScreenPosition(i2d::CVector2d(lineValues.GetElement(0), lineValues.GetElement(1)));
			istd::CIndex2d endPoint = transform.GetScreenPosition(i2d::CVector2d(lineValues.GetElement(2), lineValues.GetElement(3)));

			boundingBox.Union(startPoint);
			boundingBox.Union(endPoint);
		}

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(iview::IColorShema::TT_INACTIVE);

		return boundingBox.GetExpanded(tickerBox);
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iqtipr


