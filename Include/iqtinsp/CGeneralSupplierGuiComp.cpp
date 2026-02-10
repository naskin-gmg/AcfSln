// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtinsp/CGeneralSupplierGuiComp.h>


namespace iqtinsp
{


CGeneralSupplierGuiComp::CGeneralSupplierGuiComp()
{
}


// protected methods

void CGeneralSupplierGuiComp::ShowResultsBox(bool on)
{
	BaseClass::ResultsGroupBox->setVisible(on);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CGeneralSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(TestButton, SIGNAL(clicked()), this, SLOT(OnTestButtonClicked()), Qt::QueuedConnection);
	connect(this, SIGNAL(OnSupplierParamsChangedSignal()), SLOT(OnTestButtonClicked()), Qt::QueuedConnection);
}


void CGeneralSupplierGuiComp::OnGuiDestroyed()
{
	disconnect(TestButton, SIGNAL(clicked()), this, SLOT(OnTestButtonClicked()));

	BaseClass::OnGuiDestroyed();
}


void CGeneralSupplierGuiComp::OnGuiDesignChanged()
{
	BaseClass::OnGuiDesignChanged();

	AutoTestButton->setIcon(GetIcon(":/Icons/AutoUpdate"));
	TestButton->setIcon(GetIcon(":/Icons/Play"));
}


void CGeneralSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet & changeSet)
{
	BaseClass::UpdateGui(changeSet);

	const bool enableAutoTest = !m_disableAutoTestAttrPtr.IsValid() || !*m_disableAutoTestAttrPtr;
	AutoTestButton->setEnabled(enableAutoTest);
	AutoTestButton->setVisible(enableAutoTest);

	if (m_buttonLabelCompPtr.IsValid()) {
		TestButton->setText(m_buttonLabelCompPtr->GetText());
	}
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

void CGeneralSupplierGuiComp::OnSupplierParamsChanged()
{
	if (BaseClass::IsGuiCreated() && BaseClass::AutoTestButton->isChecked())
		Q_EMIT OnSupplierParamsChangedSignal();
}


// protected slots

void CGeneralSupplierGuiComp::OnTestButtonClicked()
{
	Test();
	QCoreApplication::removePostedEvents(this, QEvent::MetaCall); // prevents endless testing if test button is clicked continuously
}


void CGeneralSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CGeneralSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


} // namespace iqtinsp


