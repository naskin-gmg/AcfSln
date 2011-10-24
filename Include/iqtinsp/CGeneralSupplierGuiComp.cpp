#include "iqtinsp/CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


// protected slots

void CGeneralSupplierGuiComp::on_TestButton_clicked()
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() >= iproc::ISupplier::WS_ERROR){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Processing Error"));
		}
	}
}


void CGeneralSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CGeneralSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CGeneralSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CGeneralSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CGeneralSupplierGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	QString statusText = tr("Unknown");

	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		int workStatus = supplierPtr->GetWorkStatus();

		switch (workStatus){
		case iproc::ISupplier::WS_NONE:
			statusText = tr("None");
			break;

		case iproc::ISupplier::WS_INIT:
			statusText = tr("Init");
			break;

		case iproc::ISupplier::WS_LOCKED:
			statusText = tr("Locked");
			break;

		case iproc::ISupplier::WS_OK:
			statusText = tr("OK");
			break;

		case iproc::ISupplier::WS_CANCELED:
			statusText = tr("Canceled");
			break;

		case iproc::ISupplier::WS_ERROR:
			statusText = tr("Error");
			break;

		case iproc::ISupplier::WS_CRITICAL:
			statusText = tr("Critical");
			break;
		}
	}

	StatusLabel->setText(statusText);
}


} // namespace iqtinsp


