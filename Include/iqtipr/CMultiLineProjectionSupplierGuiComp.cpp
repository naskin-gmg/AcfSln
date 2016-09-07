#include "iqtipr/CMultiLineProjectionSupplierGuiComp.h"


// Qt includes
#include <QtCore/QTimer>


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

	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CMultiLineProjectionSupplierGuiComp::on_ProjectionSlider_valueChanged(int index)
{
	OnProjectionIndexChanged(index);

	UpdateGui(istd::IChangeable::GetAllChanges());
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


void CMultiLineProjectionSupplierGuiComp::OnGuiHidden()
{
	AutoUpdateButton->setChecked(false);

	BaseClass::OnGuiHidden();
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


void CMultiLineProjectionSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	istd::CChangeNotifier changePtr(&m_projectionData);
	m_projectionData.ResetSequence();

	const imeas::IMultiDataSequenceProvider* providerPtr = dynamic_cast<const imeas::IMultiDataSequenceProvider*>(GetObservedObject());
	if (providerPtr != NULL){
		int count = providerPtr->GetSequencesCount();

		if (count != ProjectionSlider->maximum() && (count > 0)){
			ProjectionSlider->setMaximum(count - 1);
			ProjectionSpin->setMaximum(count - 1);
		}
		else if (count == 0) {
			ProjectionSlider->setMaximum(0);
			ProjectionSpin->setMaximum(0);
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
	if (IsGuiCreated() && AutoUpdateButton->isChecked()){
		QTimer::singleShot(1, this, SLOT(on_TestButton_clicked()));
	}
}


QWidget* CMultiLineProjectionSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// private methods

void CMultiLineProjectionSupplierGuiComp::OnProjectionIndexChanged(int index)
{
	if (m_projectionSelectorCompPtr.IsValid() && (m_projectionSelectorCompPtr->GetSelectedOptionIndex() != index)){
		m_projectionSelectorCompPtr->SetSelectedOptionIndex(index);
	}
}


} // namespace iqtipr


