#include "iqtinsp/CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


CGeneralSupplierGuiComp::CGeneralSupplierGuiComp()
{
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


