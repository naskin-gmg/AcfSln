#include <iqtmeas/CNumericParamsGuiComp.h>


// ACF includes
#include <imeas/INumericConstraints.h>


namespace iqtmeas
{


// public methods

CNumericParamsGuiComp::~CNumericParamsGuiComp()
{
	Q_ASSERT(m_valueWidgets.IsEmpty());
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CNumericParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	imeas::INumericValue* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	imod::IModel* contraintsModelPtr = dynamic_cast<imod::IModel*>(const_cast<imeas::INumericConstraints*>(objectPtr->GetNumericConstraints()));
	if (contraintsModelPtr != NULL){
		BaseClass2::RegisterModel(contraintsModelPtr);
	}
}


void CNumericParamsGuiComp::OnGuiModelDetached()
{
	BaseClass2::UnregisterAllModels();

	BaseClass::OnGuiModelDetached();
}


void CNumericParamsGuiComp::UpdateModel() const
{
	imeas::INumericValue* objectPtr = GetObservedObject();
	if ((objectPtr != NULL) && IsGuiCreated()){
		int valuesCount = int(m_valueWidgets.GetCount());
		imath::CVarVector values(valuesCount);

		for (int i = 0; i < valuesCount; i++){
			CNumericValueWidget* valueWidgetPtr = m_valueWidgets.GetAt(i);

			values.SetElement(i, valueWidgetPtr->GetValue());
		}

		objectPtr->SetValues(values);
	}
}


void CNumericParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	QWidget* panelPtr = GetQtWidget();
	Q_ASSERT(panelPtr != NULL);	// called inside OnGuiCreated(), widget must be defined.

	QLayout* layoutPtr = panelPtr->layout();
	Q_ASSERT(layoutPtr != NULL); // layout is allways set in OnGuiCreated()

	QObjectList children = layoutPtr->children();
	for (		QObjectList::ConstIterator iter = children.constBegin();
				iter != children.constEnd();
				++iter){
		QObject* childPtr = *iter;

		delete childPtr;
	}

	imeas::INumericValue* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const imeas::INumericConstraints* constraintsPtr = objectPtr->GetNumericConstraints();

		imath::CVarVector values = objectPtr->GetValues();

		int widgetsCount = m_valueWidgets.GetCount();

		int valuesCount;
		if (constraintsPtr != NULL){
			const iprm::IOptionsList& valueListInfo = constraintsPtr->GetValueListInfo();

			valuesCount = valueListInfo.GetOptionsCount();
		}
		else{
			valuesCount = values.GetElementsCount();
		}

		m_valueWidgets.SetCount(valuesCount);

		int sliderFlags = 0;
		if (*m_isSliderVisibleAttrPtr || *m_isSliderOnlyAttrPtr){
			if (*m_isSliderVisibleAttrPtr){
				sliderFlags = CNumericValueWidget::SF_SLIDER_VISIBLE;
			}
			if (*m_isSliderOnlyAttrPtr){
				sliderFlags = CNumericValueWidget::SF_SLIDER_VISIBLE | CNumericValueWidget::SF_WITHOUT_SPIN_BOX;
			}

			if (*m_isButtonsVisibleAttrPtr){
				sliderFlags |= CNumericValueWidget::SF_SLIDER_BUTTONS;
			}

			if (*m_isSingleRowAttrPtr){
				sliderFlags |= CNumericValueWidget::SF_SINGLE_ROW;
			}
		}

		for (int i = widgetsCount; i < valuesCount; i++){
			CNumericValueWidget* valueWidgetPtr = new CNumericValueWidget(
						panelPtr, 
						sliderFlags,
						m_inputPolicyAttrPtr.IsValid()? *m_inputPolicyAttrPtr: -1,
						*m_editorPrecisionAttrPtr,
						*m_postValidationEnabledAttrPtr);

			if(		m_sliderTickPositionAttrPtr.IsValid() ||
					m_sliderTickIntervalAttrPtr.IsValid()){

				QSlider::TickPosition sliderTickPosition = QSlider::NoTicks;
				if (m_sliderTickPositionAttrPtr.IsValid()){
					switch (*m_sliderTickPositionAttrPtr){
					case 0:
						sliderTickPosition = QSlider::NoTicks;
						break;
					case 1:
					sliderTickPosition = QSlider::TicksAbove;
						break;
					case 2:
					sliderTickPosition = QSlider::TicksBelow;
						break;
					case 3:
						sliderTickPosition = QSlider::TicksBothSides;
					break;
					default:
						break;
					}
				}

				int sliderTickInterval = 0;
				if (m_sliderTickIntervalAttrPtr.IsValid()){
					sliderTickInterval = *m_sliderTickIntervalAttrPtr;
				}

				valueWidgetPtr->SetSliderTickPositionInterval(sliderTickPosition, sliderTickInterval);
			}

			connect(valueWidgetPtr, SIGNAL(ValueChanged()), this, SLOT(OnValueChanged()));

			m_valueWidgets.SetElementAt(i, valueWidgetPtr);

			layoutPtr->addWidget(valueWidgetPtr);
		}

		for (int i = 0; i < valuesCount; i++){
			CNumericValueWidget* valueWidgetPtr = m_valueWidgets.GetAt(i);
			Q_ASSERT(valueWidgetPtr != NULL);

			if (constraintsPtr != NULL){
			const iprm::IOptionsList& valueListInfo = constraintsPtr->GetValueListInfo();

			valueWidgetPtr->SetUnitInfo(
							valueListInfo.GetOptionName(i),
							valueListInfo.GetOptionDescription(i),
							constraintsPtr->GetNumericValueUnitInfo(i));
			}

			if (i < values.GetElementsCount()){
				valueWidgetPtr->SetValue(values.GetElement(i));
			}
			else{
				valueWidgetPtr->SetValue(0);
			}
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CNumericParamsGuiComp::OnGuiCreated()
{
	QWidget* panelPtr = GetQtWidget();
	Q_ASSERT(panelPtr != NULL);	// called inside OnGuiCreated(), widget must be defined.

	QLayout* layoutPtr = NULL;
	if (*m_verticalLayoutAttrPtr){
		layoutPtr = new QVBoxLayout;
	}
	else{
		layoutPtr = new QHBoxLayout;
	}

	layoutPtr->setContentsMargins(0,0,0,0);
	panelPtr->setLayout(layoutPtr);

	BaseClass::OnGuiCreated();
}


void CNumericParamsGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();

	m_valueWidgets.Reset();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CNumericParamsGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& changeSet)
{
	UpdateGui(changeSet);
}


// protected slots

void CNumericParamsGuiComp::OnValueChanged()
{
	DoUpdateModel();
}


} // namespace iqtmeas


