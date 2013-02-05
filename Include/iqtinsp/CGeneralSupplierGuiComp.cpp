#include "iqtinsp/CGeneralSupplierGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtinsp
{


// protected slots

void CGeneralSupplierGuiComp::on_TestButton_clicked()
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() >= iproc::ISupplier::WS_ERROR){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Processing Error"));

			if (AutoTestButton->isChecked()){
				AutoTestButton->setChecked(false);
			}
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
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


void CGeneralSupplierGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoTestButton->isChecked()){
		DoTest();
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CGeneralSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CGeneralSupplierGuiComp::OnGuiHidden()
{
	AutoTestButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


void CGeneralSupplierGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	Q_ASSERT(IsGuiCreated());

	QString statusLabelText = tr("Unknown");

	QString description;

	const iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);

		int workStatus = supplierPtr->GetWorkStatus();

		switch (workStatus){
		case iproc::ISupplier::WS_INVALID:
			statusLabelText = tr("None");
			break;

		case iproc::ISupplier::WS_INIT:
			statusLabelText = tr("Init");
			break;

		case iproc::ISupplier::WS_LOCKED:
			statusLabelText = tr("Locked");
			break;

		case iproc::ISupplier::WS_OK:
			if (infoProviderPtr != NULL){
				switch (infoProviderPtr->GetInformationCategory()){
				case istd::IInformationProvider::IC_WARNING:
					statusLabelText = tr("Warning");
					break;

				case istd::IInformationProvider::IC_ERROR:
					statusLabelText = tr("Error");
					break;

				default:
					statusLabelText = tr("OK");
					break;
				}
			}
			else{
				statusLabelText = tr("OK");
			}

			break;

		case iproc::ISupplier::WS_CANCELED:
			statusLabelText = tr("Canceled");
			break;

		case iproc::ISupplier::WS_ERROR:
			statusLabelText = tr("Not processed");
			break;

		case iproc::ISupplier::WS_CRITICAL:
			statusLabelText = tr("Critical");
			break;
		}

		if (infoProviderPtr != NULL){
			description = infoProviderPtr->GetInformationDescription();
		}

		UpdateAllViews();
	}

	StatusLabel->setText(statusLabelText);
	if (!description.isEmpty()){
		DescriptionLabel->setText(description);
		DescriptionLabel->setVisible(true);
	}
	else{
		DescriptionLabel->setVisible(false);
	}
}


} // namespace iqtinsp


