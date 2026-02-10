// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtmeas/CDataSequenceSupplierResultsViewComp.h>


namespace iqtmeas
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CDataSequenceSupplierResultsViewComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	if (changeSet.Contains(iinsp::ISupplier::CF_SUPPLIER_RESULTS)){
		imeas::IDataSequenceProvider* providerPtr = dynamic_cast<imeas::IDataSequenceProvider*>(GetObservedModel());
		if (providerPtr != NULL){
			const imod::IModel* productModelPtr = dynamic_cast<const imod::IModel*>(providerPtr->GetDataSequence());
			if ((productModelPtr != NULL) && m_resultsObserverCompPtr.IsValid()){
				if (!productModelPtr->IsAttached(m_resultsObserverCompPtr.GetPtr())){
					(const_cast<imod::IModel*>(productModelPtr))->AttachObserver(m_resultsObserverCompPtr.GetPtr());
				}
			}
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CDataSequenceSupplierResultsViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetWidget();

	QVBoxLayout* layoutPtr = new QVBoxLayout(widgetPtr);
	layoutPtr->setContentsMargins(0, 0, 0, 0);

	if (m_resultsGuiCompPtr.IsValid() && m_resultsObserverCompPtr.IsValid()){
		m_resultsGuiCompPtr->CreateGui(widgetPtr);
	}
}


void CDataSequenceSupplierResultsViewComp::OnGuiDestroyed()
{
	if (m_resultsGuiCompPtr.IsValid()){
		m_resultsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtmeas


