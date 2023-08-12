#include <iqtinsp/CGeneralSupplierGuiComp.h>


namespace iqtinsp
{


CGeneralSupplierGuiComp::CGeneralSupplierGuiComp()
{
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CGeneralSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ResultsGroupBox->setVisible(*m_showResultStatusFrameAttrPtr);
}


void CGeneralSupplierGuiComp::OnGuiDesignChanged()
{
	BaseClass::OnGuiDesignChanged();

	AutoTestButton->setIcon(GetIcon(":/Icons/AutoUpdate"));
	TestButton->setIcon(GetIcon(":/Icons/Play"));
}


// protected slots

void CGeneralSupplierGuiComp::on_TestButton_clicked()
{
	Test();
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


