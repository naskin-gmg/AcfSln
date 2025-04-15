#ifndef iqtinsp_TCommonSupplierGuiCompBase_included
#define iqtinsp_TCommonSupplierGuiCompBase_included


// Qt includes
#include<QtCore/QtGlobal>
#include<QtCore/QMap>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CChangeGroup.h>
#include <istd/TDelPtr.h>
#include <ifile/IFilePersistence.h>
#include <imod/IObserver.h>
#include <iprm/IParamsSet.h>
#include <iinsp/ISupplier.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <iqtinsp/iqtinsp.h>


namespace iqtinsp
{


template <class UI>
class TCommonSupplierGuiCompBase: public iqtgui::TDesignerGuiObserverCompBase<UI, iinsp::ISupplier>
{
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<UI, iinsp::ISupplier> BaseClass;

	I_BEGIN_BASE_COMPONENT(TCommonSupplierGuiCompBase);
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN_TO(m_paramsSetObserverCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN(m_connectParametersToEditorAttrPtr, "ConnectParametersToEditor", "If enabled, the parameter set of the supplier will be connected to the parameter editor", true, true);
	I_END_COMPONENT;

	TCommonSupplierGuiCompBase();

protected:
	using BaseClass::GetObservedObject;

	/**
		Check, if parameters are correct connected to GUI and can be editable.
	*/
	bool AreParamsEditable() const;

	/**
		Check if parameter loading is supported.
	*/
	bool IsLoadParamsSupported() const;

	/**
		Check if parameter storing is supported.
	*/
	bool IsSaveParamsSupported() const;

	/**
		Load parameters from external source (file).
	*/
	bool LoadParams();

	/**
		Save parameters for example to file.
	*/
	bool SaveParams();

	/**
		Do inspection test.
	*/
	bool DoTest();

	/**
		Update visual status of supplier state.
	*/
	virtual void UpdateVisualStatus();

	/**
		Method will be called every time if the parameter set of the supplier has been changed.
		Default implementation does nothing.
	*/
	virtual void OnSupplierParamsChanged();

	/**
		This method will be called after the supplier's output has been changed and new results are available.
	*/
	virtual void BeforeSupplierGuiUpdated();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	// abstract methods
	/**
		Get parameters widget object.
		Extern paremeters GUI will attach to this widget.
		This widget should have some layout.
	*/
	virtual QWidget* GetParamsWidget() const = 0;

protected:
	class ParamsObserver: public imod::CSingleModelObserverBase
	{
	public:
		ParamsObserver(TCommonSupplierGuiCompBase* parentPtr);

		using imod::CSingleModelObserverBase::EnsureModelDetached;

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		TCommonSupplierGuiCompBase& m_parent;
	};

	class StatusObserver: public imod::CSingleModelObserverBase
	{
	public:
		StatusObserver(TCommonSupplierGuiCompBase* parentPtr);

		using imod::CSingleModelObserverBase::EnsureModelDetached;

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		TCommonSupplierGuiCompBase& m_parent;
	};

	ParamsObserver m_paramsObserver;
	StatusObserver m_statusObserver;

protected:
	using BaseClass::m_visualStatus;

private:
	I_REF(ifile::IFilePersistence, m_bitmapLoaderCompPtr);
	I_REF(ifile::IFilePersistence, m_paramsLoaderCompPtr);

	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);

	I_ATTR(bool, m_connectParametersToEditorAttrPtr);

	bool m_areParamsEditable;
};


// public methods

template <class UI>
TCommonSupplierGuiCompBase<UI>::TCommonSupplierGuiCompBase()
	:m_paramsObserver(this),
	m_statusObserver(this),
	m_areParamsEditable(false)
{
	BaseClass::SetStatusIcon(QIcon(":/Icons/StateUnknown"));
	BaseClass::SetStatusText("Not yet processed");
}


// protected methods

template <class UI>
bool TCommonSupplierGuiCompBase<UI>::AreParamsEditable() const
{
	return m_areParamsEditable;
}


template <class UI>
bool TCommonSupplierGuiCompBase<UI>::IsLoadParamsSupported() const
{
	const iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->IsOperationSupported(
						paramsPtr,
						NULL,
						ifile::IFilePersistence::QF_LOAD | ifile::IFilePersistence::QF_ANONYMOUS)){
				return true;
			}
		}
	}

	return false;
}


template <class UI>
bool TCommonSupplierGuiCompBase<UI>::IsSaveParamsSupported() const
{
	const iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->IsOperationSupported(
						paramsPtr,
						NULL,
						ifile::IFilePersistence::QF_SAVE | ifile::IFilePersistence::QF_ANONYMOUS)){
				return true;
			}
		}
	}

	return false;
}


template <class UI>
bool TCommonSupplierGuiCompBase<UI>::LoadParams()
{
	iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->LoadFromFile(*paramsPtr, "") != ifile::IFilePersistence::OS_FAILED){
				return true;
			}
			else{
				QMessageBox::warning(
							BaseClass::GetQtWidget(),
							QObject::tr("Error"),
							QObject::tr("Cannot load parameters"));
			}
		}
	}

	return false;
}


template <class UI>
bool TCommonSupplierGuiCompBase<UI>::SaveParams()
{
	const iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->SaveToFile(*paramsPtr, "") != ifile::IFilePersistence::OS_FAILED){
				return true;
			}
			else{
				QMessageBox::warning(
							BaseClass::GetQtWidget(),
							QObject::tr("Error"),
							QObject::tr("Cannot save parameters"));
			}
		}
	}

	return false;
}


template <class UI>
bool TCommonSupplierGuiCompBase<UI>::DoTest()
{
	iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if (supplierPtr != NULL){
		istd::CChangeGroup changeGroup(supplierPtr);

		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		return supplierPtr->GetWorkStatus() != iinsp::ISupplier::WS_FAILED;
	}

	return false;
}


template <class UI>
void TCommonSupplierGuiCompBase<UI>::UpdateVisualStatus()
{
	QString statusText = "";
	static QIcon unknownIcon(":/Icons/StateUnknown");
	static QIcon warningIcon(":/Icons/StateWarning");
	static QIcon invalidIcon(":/Icons/StateInvalid");
	static QIcon okIcon(":/Icons/StateOk");

	QIcon statusIcon = unknownIcon;

	QString description;

	const iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if (supplierPtr != NULL){
		const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);
		if (infoProviderPtr == NULL){
			infoProviderPtr =  CompCastPtr<const istd::IInformationProvider>(supplierPtr);
		}

		int category = (infoProviderPtr == NULL) ? istd::IInformationProvider::IC_INFO : infoProviderPtr->GetInformationCategory();

		int workStatus = supplierPtr->GetWorkStatus();

		switch (workStatus){
		case iinsp::ISupplier::WS_LOCKED:
			statusText = QObject::tr("Locked");
			break;

		case iinsp::ISupplier::WS_OK:
			switch (category){
				case istd::IInformationProvider::IC_WARNING:
					statusText = QObject::tr("Processing completed with warnings");
					statusIcon = warningIcon;
					break;

				case istd::IInformationProvider::IC_ERROR:
					statusText = QObject::tr("Processing completed with errors");
					statusIcon = invalidIcon;
					break;

				default:
					statusText = QObject::tr("Processing completed without errors");
					statusIcon = okIcon;
					break;
				}
			break;

		case iinsp::ISupplier::WS_CANCELED:
			statusText = QObject::tr("Processing canceled by user");
			break;

		case iinsp::ISupplier::WS_FAILED:
			statusText = QObject::tr("Processing not possible");
			statusIcon = invalidIcon;
			break;

		default:
			break;
		}
	}

	istd::CChangeNotifier visualStatusNotifier(&m_visualStatus);
	BaseClass::SetStatusIcon(statusIcon);
	BaseClass::SetStatusText(statusText);
}


template <class UI>
void TCommonSupplierGuiCompBase<UI>::OnSupplierParamsChanged()
{
}


template <class UI>
void TCommonSupplierGuiCompBase<UI>::BeforeSupplierGuiUpdated()
{
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class UI>
void TCommonSupplierGuiCompBase<UI>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	Q_ASSERT(supplierPtr != NULL);	// model must be attached

	iprm::IParamsSet* paramsPtr = const_cast<iprm::IParamsSet*>(supplierPtr->GetModelParametersSet());
	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(paramsPtr);

	m_areParamsEditable = !*m_connectParametersToEditorAttrPtr;
	bool areParamsAttachedToEditor = !*m_connectParametersToEditorAttrPtr;
	QWidget* paramsWidget = GetParamsWidget();
	if (paramsWidget != NULL){
		if (m_paramsSetGuiCompPtr.IsValid()){
			m_paramsSetGuiCompPtr->CreateGui(paramsWidget);
		}

		if (*m_connectParametersToEditorAttrPtr && (paramsModelPtr != NULL) && m_paramsSetObserverCompPtr.IsValid()){
			areParamsAttachedToEditor = paramsModelPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
			if (!areParamsAttachedToEditor){
				qWarning("Supplier parameters could not be connected to the editor");
			}

			m_areParamsEditable = true;


			// Attach internal parameter observer:
			paramsModelPtr->AttachObserver(&m_paramsObserver);
		}

		paramsWidget->setVisible(m_areParamsEditable);
		paramsWidget->setEnabled(areParamsAttachedToEditor);
	}

	imod::IModel* statusModelPtr = supplierPtr->GetWorkStatusModel();
	if (statusModelPtr != NULL){
		statusModelPtr->AttachObserver(&m_statusObserver);
	}

	BaseClass::SetStatusIcon(QIcon(":/Icons/StateUnknown"));
}


template <class UI>
void TCommonSupplierGuiCompBase<UI>::OnGuiModelDetached()
{
	iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	Q_ASSERT(supplierPtr != NULL);	// model must be attached

	iprm::IParamsSet* paramsPtr = const_cast<iprm::IParamsSet*>(supplierPtr->GetModelParametersSet());
	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(paramsPtr);

	if (		m_paramsSetObserverCompPtr.IsValid() &&
				(paramsModelPtr != NULL) &&
				paramsModelPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		paramsModelPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}

	m_areParamsEditable = false;

	m_paramsObserver.EnsureModelDetached();

	m_statusObserver.EnsureModelDetached();

	BaseClass::OnGuiModelDetached();
}


// reimplemented (imod::IObserver)

template <class UI>
void TCommonSupplierGuiCompBase<UI>::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	if (!BaseClass::IsGuiCreated() || changeSet.ContainsExplicit(istd::IChangeable::CF_DESTROYING)){
		BaseClass::AfterUpdate(modelPtr, changeSet);

		return;
	}

	UpdateVisualStatus();

	BeforeSupplierGuiUpdated();

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// public methods of embedded class ParamsObserver

template <class UI>
TCommonSupplierGuiCompBase<UI>::ParamsObserver::ParamsObserver(TCommonSupplierGuiCompBase* parentPtr)
	:m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

template <class UI>
void TCommonSupplierGuiCompBase<UI>::ParamsObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.OnSupplierParamsChanged();
}


// public methods of embedded class StatusObserver

template <class UI>
TCommonSupplierGuiCompBase<UI>::StatusObserver::StatusObserver(TCommonSupplierGuiCompBase* parentPtr)
	:m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

template <class UI>
void TCommonSupplierGuiCompBase<UI>::StatusObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.UpdateVisualStatus();
}


} // namespace iqtinsp


#endif // !iqtinsp_TCommonSupplierGuiCompBase_included


