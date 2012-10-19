#include "iqtipr/CMultiLineProjectionSupplierGuiComp.h"


namespace iqtipr
{


// protected slots

void CMultiLineProjectionSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CMultiLineProjectionSupplierGuiComp::on_ProjectionSpin_valueChanged(int index)
{
	OnProjectionIndexChanged(index);
}


void CMultiLineProjectionSupplierGuiComp::on_ProjectionSlider_valueChanged(int index)
{
	OnProjectionIndexChanged(index);
}


// protected methods

// reimplemented (iqtgui::IGuiObject)

void CMultiLineProjectionSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_projectionObserverGuiCompPtr.IsValid()){
		m_projectionObserverGuiCompPtr->CreateGui(ProjectionFrame);
	}
}


void CMultiLineProjectionSupplierGuiComp::OnGuiDestroyed()
{
	if (m_projectionObserverGuiCompPtr.IsValid()){
		m_projectionObserverGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CMultiLineProjectionSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (m_projectionObserverCompPtr.IsValid()){
		m_projectionData.AttachObserver(m_projectionObserverCompPtr.GetPtr());
	}
}


void CMultiLineProjectionSupplierGuiComp::OnGuiModelDetached()
{
	if (m_projectionObserverCompPtr.IsValid()){
		m_projectionData.DetachAllObservers();
	}

	BaseClass::OnGuiModelDetached();
}


void CMultiLineProjectionSupplierGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	I_ASSERT(IsGuiCreated());

	istd::CChangeNotifier changePtr(&m_projectionData);
	m_projectionData.ResetSequence();

	const imeas::IMultiDataSequenceProvider* providerPtr = dynamic_cast<const imeas::IMultiDataSequenceProvider*>(GetObjectPtr());
	if (providerPtr != NULL){
		int count = providerPtr->GetSequencesCount();

		if (count != ProjectionSlider->maximum()){
			ProjectionSlider->setMaximum(count - 1);
			ProjectionSpin->setMaximum(count - 1);
		}

		int currentIndex = ProjectionSlider->value();
		const imeas::IDataSequence* projectionPtr = providerPtr->GetDataSequence(currentIndex);
		if (projectionPtr != NULL){
			m_projectionData.CopyFrom(*projectionPtr);
		}
	}
	else{
		ProjectionSlider->setMaximum(0);
		ProjectionSpin->setMaximum(0);
	}
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

void CMultiLineProjectionSupplierGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated()){
		if (AutoUpdateButton->isChecked()){
			on_TestButton_clicked();
		}
	}
}


QWidget* CMultiLineProjectionSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// private methods

void CMultiLineProjectionSupplierGuiComp::OnProjectionIndexChanged(int index)
{
	if (m_projectionSelectorCompPtr.IsValid() && (m_projectionSelectorCompPtr->GetSelectedOptionIndex() != index)){
		UpdateGui();
	
		m_projectionSelectorCompPtr->SetSelectedOptionIndex(index);
	}
}


} // namespace iqtipr


