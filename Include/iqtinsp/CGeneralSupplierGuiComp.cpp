#include "iqtinsp/CGeneralSupplierGuiComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"


namespace iqtinsp
{


CGeneralSupplierGuiComp::CGeneralSupplierGuiComp()
{
	connect(this, SIGNAL(DoAutoTest()), SLOT(OnAutoTest()), Qt::QueuedConnection);
}


// protected slots

void CGeneralSupplierGuiComp::OnAutoTest()
{
	DoTest();
}


void CGeneralSupplierGuiComp::on_TestButton_clicked()
{
	iinsp::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() >= iinsp::ISupplier::WS_ERROR){
			QMessageBox::warning(
						GetQtWidget(),
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
		emit DoAutoTest();
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


void CGeneralSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	QString statusLabelText = tr("Unknown");

	QString description;

	const iinsp::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);

		int workStatus = supplierPtr->GetWorkStatus();

		switch (workStatus){
		case iinsp::ISupplier::WS_INVALID:
			statusLabelText = tr("None");
			break;

		case iinsp::ISupplier::WS_INIT:
			statusLabelText = tr("Init");
			break;

		case iinsp::ISupplier::WS_LOCKED:
			statusLabelText = tr("Locked");
			break;

		case iinsp::ISupplier::WS_OK:
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

		case iinsp::ISupplier::WS_CANCELED:
			statusLabelText = tr("Canceled");
			break;

		case iinsp::ISupplier::WS_ERROR:
			statusLabelText = tr("Not processed");
			break;

		case iinsp::ISupplier::WS_CRITICAL:
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


