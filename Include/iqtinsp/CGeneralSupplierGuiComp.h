#ifndef iqtinsp_CGeneralSupplierGuiComp_included
#define iqtinsp_CGeneralSupplierGuiComp_included


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iinsp/ISupplier.h"

// ACF-Solutions includes
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "GeneratedFiles/iqtinsp/ui_CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


template <class UI = Ui::CGeneralSupplierGuiComp>
class TGeneralSupplierGuiComp: public TSupplierGuiCompBase<UI, iinsp::ISupplier>
{
public:
	typedef TSupplierGuiCompBase<UI, iinsp::ISupplier> BaseClass;

protected:
	// pseudo slots
	virtual void Test();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiHidden();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);
};



// protected methods

template <class UI>
void TGeneralSupplierGuiComp<UI>::Test()
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


// reimplemented (iqtinsp::TSupplierGuiCompBase)

template <class UI>
QWidget* TGeneralSupplierGuiComp<UI>::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


template <class UI>
void TGeneralSupplierGuiComp<UI>::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoTestButton->isChecked()){
		QTimer::singleShot(1, this, SLOT(on_TestButton_clicked()));
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class UI>
void TGeneralSupplierGuiComp<UI>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


template <class UI>
void TGeneralSupplierGuiComp<UI>::OnGuiHidden()
{
	AutoTestButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


template <class UI>
void TGeneralSupplierGuiComp<UI>::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
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


/**
	Default implementation of the supplier's UI
*/
class CGeneralSupplierGuiComp: public TGeneralSupplierGuiComp<>
{
	Q_OBJECT

public:
	typedef TGeneralSupplierGuiComp<> BaseClass;

	I_BEGIN_COMPONENT(CGeneralSupplierGuiComp);
	I_END_COMPONENT;

	CGeneralSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
};


} // namespace iqtinsp


#endif // !iqtinsp_CGeneralSupplierGuiComp_included


