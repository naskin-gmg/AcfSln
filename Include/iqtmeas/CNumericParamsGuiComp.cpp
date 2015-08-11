#include "iqtmeas/CNumericParamsGuiComp.h"


// ACF includes
#include "imeas/INumericConstraints.h"


namespace iqtmeas
{


// public methods

CNumericParamsGuiComp::~CNumericParamsGuiComp()
{
	Q_ASSERT(m_valueWidgets.IsEmpty());
}


// reimplemented (imod::IModelEditor)

void CNumericParamsGuiComp::UpdateModel() const
{
	imeas::INumericValue* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && IsGuiCreated()){
		int valuesCount = int(m_valueWidgets.GetCount());
		imath::CVarVector values(valuesCount);

		for (int i = 0; i < valuesCount; i++){
			CNumericValueWidget* valueWidgetPtr = m_valueWidgets.GetAt(i);

			values.SetElement(i, valueWidgetPtr->GetValue());
		}

		objectPtr->SetValues(values);
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CNumericParamsGuiComp::OnGuiDestroyed()
{
	m_valueWidgets.Reset();

	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CNumericParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	imeas::INumericValue* objectPtr = GetObjectPtr();
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


void CNumericParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	QWidget* panelPtr = GetQtWidget();
	Q_ASSERT(panelPtr != NULL);	// called inside UpdateGui(), widget must be defined.

	QLayout* layoutPtr = NULL;
	if (*m_verticalLayoutAttrPtr){
		layoutPtr = new QVBoxLayout;
	}
	else{
		layoutPtr = new QHBoxLayout;
	}

	layoutPtr->setMargin(0);
	panelPtr->setLayout(layoutPtr);

	imeas::INumericValue* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imeas::INumericConstraints* constraintsPtr = objectPtr->GetNumericConstraints();

		imath::CVarVector values = objectPtr->GetValues();

		int widgetsCount = m_valueWidgets.GetCount();

		int valuesCount;
		if (constraintsPtr != NULL){
			valuesCount = constraintsPtr->GetNumericValuesCount();
		}
		else{
			valuesCount = values.GetElementsCount();
		}

		m_valueWidgets.SetCount(valuesCount);
		for (int i = widgetsCount; i < valuesCount; i++){
			CNumericValueWidget* valueWidgetPtr = new CNumericValueWidget(
						panelPtr, 
						*m_isSliderVisibleAttrPtr, 
						*m_isButtonsVisibleAttrPtr,
						m_inputPolicyAttrPtr.IsValid()? *m_inputPolicyAttrPtr: -1);
			
			connect(valueWidgetPtr, SIGNAL(ValueChanged()), this, SLOT(OnValueChanged()));

			m_valueWidgets.SetElementAt(i, valueWidgetPtr);

			layoutPtr->addWidget(valueWidgetPtr);
		}

		for (int i = 0; i < valuesCount; i++){
			CNumericValueWidget* valueWidgetPtr = m_valueWidgets.GetAt(i);
			Q_ASSERT(valueWidgetPtr != NULL);

			if (constraintsPtr != NULL){
				valueWidgetPtr->SetUnitInfo(
							constraintsPtr->GetNumericValueName(i),
							constraintsPtr->GetNumericValueUnitInfo(i));
			}

			valueWidgetPtr->SetupValueEditor(*m_editorPrecisionAttrPtr, *m_editorStepValueAttrPtr);

			if (i < values.GetElementsCount()){
				valueWidgetPtr->SetValue(values.GetElement(i));
			}
			else{
				valueWidgetPtr->SetValue(0);
			}
		}
	}
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


