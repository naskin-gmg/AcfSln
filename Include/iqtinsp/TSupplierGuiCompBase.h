#ifndef iqtinsp_TSupplierGuiCompBase_included
#define iqtinsp_TSupplierGuiCompBase_included


// Qt includes
#include <QtGui/QMessageBox>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IFileLoader.h"
#include "imod/IObserver.h"
#include "iprm/IParamsSet.h"
#include "i2d/ICalibrationProvider.h"
#include "iproc/ISupplier.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqt2d/TViewExtenderCompBase.h"

#include "iqtinsp/iqtinsp.h"


namespace iqtinsp
{


template <class UI, class WidgetType = QWidget>
class TSupplierGuiCompBase:
			public iqt2d::TViewExtenderCompBase<
						iqtgui::TDesignerGuiObserverCompBase<UI, iproc::ISupplier> >,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iqt2d::TViewExtenderCompBase<
					iqtgui::TDesignerGuiObserverCompBase<UI, iproc::ISupplier> > BaseClass;

	I_BEGIN_BASE_COMPONENT(TSupplierGuiCompBase);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN_TO(m_paramsSetObserverCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN_TO(m_paramsSetExtenderCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN(m_connectParametersToEditorAttrPtr, "ConnectParametersToEditor", "If enabled, the parameter set of the supplier will be connected to the parameter editor", true, true);
	I_END_COMPONENT;

	TSupplierGuiCompBase();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	typedef typename BaseClass::Shapes Shapes;

	using BaseClass::GetObjectPtr;

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
		Method will be called every time if the parameter set of the supplier has been changed.
		Default implementation does nothing.
	*/
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

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
		ParamsObserver(TSupplierGuiCompBase* parentPtr);

		using imod::CSingleModelObserverBase::EnsureModelDetached;

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		TSupplierGuiCompBase& m_parent;
	};

	ParamsObserver m_paramsObserver;

private:
	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::IViewExtender, m_paramsSetExtenderCompPtr);

	I_ATTR(bool, m_connectParametersToEditorAttrPtr);

	bool m_areParamsEditable;

	using BaseClass::m_visualStatus;
};


// public methods

template <class UI, class WidgetType>
TSupplierGuiCompBase<UI, WidgetType>::TSupplierGuiCompBase()
	:m_paramsObserver(this),
	m_areParamsEditable(false)
{
}


// reimplemented (iqt2d::IViewExtender)

template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	BaseClass::AddItemsToScene(providerPtr, flags);

	if ((flags & iqt2d::IViewExtender::SF_DIRECT) != 0){
		if (m_paramsSetExtenderCompPtr.IsValid()){
			m_paramsSetExtenderCompPtr->AddItemsToScene(providerPtr, flags);
		}
	}
}


template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// reimplemented (i2d::ICalibrationProvider)

template <class UI, class WidgetType>
const i2d::ICalibration2d* TSupplierGuiCompBase<UI, WidgetType>::GetCalibration() const
{
	i2d::ICalibrationProvider* calibrationProviderPtr = CompCastPtr<i2d::ICalibrationProvider>(GetObjectPtr());
	if (calibrationProviderPtr != NULL){
		return calibrationProviderPtr->GetCalibration();
	}

	return NULL;
}


// protected methods

template <class UI, class WidgetType>
bool TSupplierGuiCompBase<UI, WidgetType>::AreParamsEditable() const
{
	return m_areParamsEditable;
}


template <class UI, class WidgetType>
bool TSupplierGuiCompBase<UI, WidgetType>::IsLoadParamsSupported() const
{
	const iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->IsOperationSupported(
						paramsPtr,
						NULL,
						iser::IFileLoader::QF_LOAD | iser::IFileLoader::QF_ANONYMOUS)){
				return true;
			}
		}
	}

	return false;
}


template <class UI, class WidgetType>
bool TSupplierGuiCompBase<UI, WidgetType>::IsSaveParamsSupported() const
{
	const iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->IsOperationSupported(
						paramsPtr,
						NULL,
						iser::IFileLoader::QF_SAVE | iser::IFileLoader::QF_ANONYMOUS)){
				return true;
			}
		}
	}

	return false;
}


template <class UI, class WidgetType>
bool TSupplierGuiCompBase<UI, WidgetType>::LoadParams()
{
	iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->LoadFromFile(*paramsPtr, "") != iser::IFileLoader::StateFailed){
				return true;
			}
			else{
				QMessageBox::information(
							NULL,
							QObject::tr("Error"),
							QObject::tr("Cannot load parameters"));
			}
		}
	}

	return false;
}


template <class UI, class WidgetType>
bool TSupplierGuiCompBase<UI, WidgetType>::SaveParams()
{
	const iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->SaveToFile(*paramsPtr, "") != iser::IFileLoader::StateFailed){
				return true;
			}
			else{
				QMessageBox::information(
							NULL,
							QObject::tr("Error"),
							QObject::tr("Cannot save parameters"));
			}
		}
	}

	return false;
}


template <class UI, class WidgetType>
bool TSupplierGuiCompBase<UI, WidgetType>::DoTest()
{
	iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		return supplierPtr->GetWorkStatus() < iproc::ISupplier::WS_ERROR;
	}

	return false;
}


template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::OnSupplierParamsChanged()
{
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	I_ASSERT(supplierPtr != NULL);	// model must be attached

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

	BaseClass::SetStatusIcon(QIcon(":/Icons/StateUnknown.svg"));
}


template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::OnGuiModelDetached()
{
	iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	I_ASSERT(supplierPtr != NULL);	// model must be attached

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

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqt2d::TViewExtenderCompBase)

template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::CreateShapes(int /*sceneId*/, Shapes& /*result*/)
{
}


// reimplemented (imod::IObserver)

template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (!BaseClass::IsGuiCreated()){
		BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);

		return;
	}

	QString statusText = "";
	QIcon statusIcon = QIcon(":/Icons/StateUnknown.svg");

	QString description;

	const iproc::ISupplier* supplierPtr = BaseClass::GetObjectPtr();
	if (supplierPtr != NULL){
		const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);

		int workStatus = supplierPtr->GetWorkStatus();

		switch (workStatus){
		case iproc::ISupplier::WS_LOCKED:
			statusText = QObject::tr("Locked");
			break;

		case iproc::ISupplier::WS_OK:
			if (infoProviderPtr != NULL){
				switch (infoProviderPtr->GetInformationCategory()){
				case istd::IInformationProvider::IC_WARNING:
					statusText = QObject::tr("Processing completed with warnings");
					statusIcon = QIcon(":/Icons/StateWarning.svg");
					break;

				case istd::IInformationProvider::IC_ERROR:
					statusText = QObject::tr("Processing completed with errors");
					statusIcon = QIcon(":/Icons/StateInvalid.svg");
					break;

				default:
					statusText = QObject::tr("Processing completed without errors");
					statusIcon = QIcon(":/Icons/StateOk.svg");
					break;
				}
			}
			else{
				statusText = QObject::tr("Processing completed without errors");
				statusIcon = QIcon(":/Icons/StateOk.svg");
			}
			break;

		case iproc::ISupplier::WS_CANCELED:
			statusText = QObject::tr("Processing canceled by user");
			break;

		case iproc::ISupplier::WS_ERROR:
			statusText = QObject::tr("Processing not possible");
			statusIcon = QIcon(":/Icons/StateInvalid.svg");
			break;

		case iproc::ISupplier::WS_CRITICAL:
			statusText = QObject::tr("Critical error occurred, application problem");
			statusIcon = QIcon(":/Icons/Error.svg");
			break;

		default:
			break;
		}
	}

	istd::CChangeNotifier visualStatusNotifier(&m_visualStatus);
	BaseClass::SetStatusIcon(statusIcon);
	BaseClass::SetStatusText(statusText);

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// public methods of embedded class ParamsObserver

template <class UI, class WidgetType>
TSupplierGuiCompBase<UI, WidgetType>::ParamsObserver::ParamsObserver(TSupplierGuiCompBase* parentPtr)
	:m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)
template <class UI, class WidgetType>
void TSupplierGuiCompBase<UI, WidgetType>::ParamsObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & istd::IChangeable::CF_MODEL) != 0){
		m_parent.OnSupplierParamsChanged();
	}
}

} // namespace iqtinsp


#endif // !iqtinsp_TSupplierGuiCompBase_included


