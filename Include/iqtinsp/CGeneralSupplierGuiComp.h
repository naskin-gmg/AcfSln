#ifndef iqtinsp_CGeneralSupplierGuiComp_included
#define iqtinsp_CGeneralSupplierGuiComp_included


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iinsp/ISupplier.h>

// ACF-Solutions includes
#include <iqtinsp/TSupplierGuiCompBase.h>
#include <GeneratedFiles/iqtinsp/ui_CGeneralSupplierGuiComp.h>


namespace iqtinsp
{


template <class UI = Ui::CGeneralSupplierGuiComp>
class TGeneralSupplierGuiComp: public TSupplierGuiCompBase<UI>
{
public:
	typedef TSupplierGuiCompBase<UI> BaseClass;

protected:
	// pseudo slots
	virtual void Test();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const override;
	virtual void OnSupplierParamsChanged() override;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiHidden() override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
};


// protected methods

template <class UI>
void TGeneralSupplierGuiComp<UI>::Test()
{
	iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if (supplierPtr != NULL){
		istd::CChangeGroup changeGroup(supplierPtr);

		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() == iinsp::ISupplier::WS_FAILED){
			QMessageBox::warning(
				BaseClass::GetQtWidget(),
				QObject::tr("Error"),
				QObject::tr("Processing Error"));

			if (BaseClass::AutoTestButton->isChecked()){
				BaseClass::AutoTestButton->setChecked(false);
			}
		}
	}
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

template <class UI>
QWidget* TGeneralSupplierGuiComp<UI>::GetParamsWidget() const
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	return BaseClass::ParamsFrame;
}


template <class UI>
void TGeneralSupplierGuiComp<UI>::OnSupplierParamsChanged()
{
	if (BaseClass::IsGuiCreated() && BaseClass::AutoTestButton->isChecked()){
		QTimer::singleShot(1, this, SLOT(on_TestButton_clicked()));
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class UI>
void TGeneralSupplierGuiComp<UI>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	bool isLoadSupported = BaseClass::IsLoadParamsSupported();
	bool isSaveSupported = BaseClass::IsSaveParamsSupported();
	bool isEditSupported = BaseClass::AreParamsEditable();

	BaseClass::ParamsGB->setVisible(isEditSupported || isLoadSupported);

	BaseClass::LoadParamsButton->setVisible(isLoadSupported);
	BaseClass::SaveParamsButton->setVisible(isSaveSupported);
}


template <class UI>
void TGeneralSupplierGuiComp<UI>::OnGuiHidden()
{
	BaseClass::AutoTestButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


template <class UI>
void TGeneralSupplierGuiComp<UI>::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(BaseClass::IsGuiCreated());

	BaseClass::UpdateVisualStatus();

	QString statusLabelText = BaseClass::tr("Unknown");

	QString description;
	const iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if (supplierPtr != NULL){
		BaseClass::UpdateAllViews();

		const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);
		if (infoProviderPtr != NULL){
			description = infoProviderPtr->GetInformationDescription();
		}
	}

	BaseClass::StatusLabel->setText(BaseClass::m_visualStatus.GetStatusText());
	if (!description.isEmpty()){
		BaseClass::DescriptionLabel->setText(description);
		BaseClass::DescriptionLabel->setVisible(true);
	}
	else{
		BaseClass::DescriptionLabel->setVisible(false);
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
		I_ASSIGN(m_showResultStatusFrameAttrPtr, "ShowResultStatusFrame", "If enabled, show result status group box on the bottom of the view", true, true);
	I_END_COMPONENT;

	CGeneralSupplierGuiComp();

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDesignChanged() override;

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

private:
	I_ATTR(bool, m_showResultStatusFrameAttrPtr);
};


} // namespace iqtinsp


#endif // !iqtinsp_CGeneralSupplierGuiComp_included


