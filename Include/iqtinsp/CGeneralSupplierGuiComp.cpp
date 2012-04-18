#include "iqtinsp/CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


// reimplemented (istd::IVisualStatusProvider)

QIcon CGeneralSupplierGuiComp::GetStatusIcon() const
{
	return m_statusIcon;
}


QString CGeneralSupplierGuiComp::GetStatusText() const
{
	return m_statusText;
}


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

	QString statusLabelText = tr("Unknown");
	m_statusText = "";
	m_statusIcon = QIcon();

	QString description;

	const iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);

		int workStatus = supplierPtr->GetWorkStatus();

		switch (workStatus){
		case iproc::ISupplier::WS_NONE:
			statusLabelText = tr("None");
			break;

		case iproc::ISupplier::WS_INIT:
			statusLabelText = tr("Init");
			break;

		case iproc::ISupplier::WS_LOCKED:
			statusLabelText = tr("Locked");
			break;

		case iproc::ISupplier::WS_OK:
			statusLabelText = tr("OK");
			m_statusText = tr("Processing completed without errors");
			if (infoProviderPtr != NULL){
				if (infoProviderPtr->GetInformationCategory() == istd::IInformationProvider::IC_WARNING){
					statusLabelText = tr("Warning");
					m_statusText = tr("Processing completed with warnings");
				}
			}
			break;

		case iproc::ISupplier::WS_CANCELED:
			statusLabelText = tr("Canceled");
			m_statusText = tr("Processing canceled by user");
			break;

		case iproc::ISupplier::WS_ERROR:
			statusLabelText = tr("Error");
			m_statusText = tr("Processing with errors");
			break;

		case iproc::ISupplier::WS_CRITICAL:
			statusLabelText = tr("Critical");
			m_statusText = tr("Critical error occurred");
			break;
		}

		if (infoProviderPtr != NULL){
			description = infoProviderPtr->GetInformationDescription();
		}
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


// static attributes

QIcon CGeneralSupplierGuiComp::s_noneIcon(":/Icons/StateNone.svg");
QIcon CGeneralSupplierGuiComp::s_okIcon(":/Icons/StateOk.svg");
QIcon CGeneralSupplierGuiComp::s_warningIcon(":/Icons/StateWarning.svg");
QIcon CGeneralSupplierGuiComp::s_errorIcon(":/Icons/StateInvalid.svg");


} // namespace iqtinsp


