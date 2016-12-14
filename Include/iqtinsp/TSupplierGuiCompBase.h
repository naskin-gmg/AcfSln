#ifndef iqtinsp_TSupplierGuiCompBase_included
#define iqtinsp_TSupplierGuiCompBase_included


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
#include <istd/TDelPtr.h>
#include <ifile/IFilePersistence.h>
#include <imod/IObserver.h>
#include <iprm/IParamsSet.h>
#include <i2d/ICalibrationProvider.h>
#include <iinsp/ISupplier.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iqt2d/TViewExtenderCompBase.h>

// ACF-Solutions includes
#include <iqtinsp/iqtinsp.h>


namespace iqtinsp
{


template <class UI>
class TSupplierGuiCompBase:
			public iqt2d::TViewExtenderCompBase<
						iqtgui::TDesignerGuiObserverCompBase<UI, iinsp::ISupplier> >
{
public:
	typedef iqt2d::TViewExtenderCompBase<
					iqtgui::TDesignerGuiObserverCompBase<UI, iinsp::ISupplier> > BaseClass;

	I_BEGIN_BASE_COMPONENT(TSupplierGuiCompBase);
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN_TO(m_paramsSetObserverCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN_TO(m_paramsSetExtenderCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN(m_connectParametersToEditorAttrPtr, "ConnectParametersToEditor", "If enabled, the parameter set of the supplier will be connected to the parameter editor", true, true);
		I_ASSIGN(m_viewCalibrationModeAttrPtr, "ViewCalibrationMode", "Control when calibration from supplier will be set to view\n\t0 - never use calibration\n\t1 - only indirect (used as slave)\n\t2 - allways set calibration if available", true, 0);
	I_END_COMPONENT;

	TSupplierGuiCompBase();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

protected:
	typedef typename BaseClass::Shapes Shapes;

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
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

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
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		TSupplierGuiCompBase& m_parent;
	};

	ParamsObserver m_paramsObserver;

protected:	
	using BaseClass::m_visualStatus;

private:
	/**
		Control mode of calibration.
	*/
	enum ViewCalibrationMode
	{
		/**
			Never set calibration to view.
		*/
		VCM_NONE,
		/**
			Set calibration to view only if indirect call.
		*/
		VCM_INDIRECT,
		/**
			Always set calibration to view, if supplier provide it.
		*/
		VCM_ALWAYS
	};

	I_REF(ifile::IFilePersistence, m_bitmapLoaderCompPtr);
	I_REF(ifile::IFilePersistence, m_paramsLoaderCompPtr);

	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::IViewExtender, m_paramsSetExtenderCompPtr);

	I_ATTR(bool, m_connectParametersToEditorAttrPtr);
	I_ATTR(int, m_viewCalibrationModeAttrPtr);

	bool m_areParamsEditable;
	typedef QMap<const iqt2d::IViewProvider*, istd::TDelPtr<i2d::ICalibration2d> > ProviderToCalibrationMap;
	ProviderToCalibrationMap m_providerToCalibrationMap;
};


// public methods

template <class UI>
TSupplierGuiCompBase<UI>::TSupplierGuiCompBase()
	:m_paramsObserver(this),
	m_areParamsEditable(false)
{
	BaseClass::SetStatusIcon(QIcon(":/Icons/StateUnknown"));
	BaseClass::SetStatusText("Not yet processed");
}


// reimplemented (iqt2d::IViewExtender)

template <class UI>
void TSupplierGuiCompBase<UI>::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	if (*m_viewCalibrationModeAttrPtr > VCM_NONE){
		if ((*m_viewCalibrationModeAttrPtr >= VCM_ALWAYS) || ((flags & iqt2d::IViewExtender::SF_DIRECT) == 0)){
			const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(GetObservedObject());
			iview::CCalibratedViewBase* viewPtr = dynamic_cast<iview::CCalibratedViewBase*>(providerPtr->GetView());

			if ((calibrationProviderPtr != NULL) && (viewPtr != NULL)){
				istd::TDelPtr<i2d::ICalibration2d> calibrationCopyPtr;

				const i2d::ICalibration2d* calibrationPtr = calibrationProviderPtr->GetCalibration();
				if (calibrationPtr != NULL){
					calibrationCopyPtr.SetCastedOrRemove(calibrationPtr->CloneMe());
				}

				viewPtr->SetDisplayCalibration(calibrationCopyPtr.GetPtr());

				m_providerToCalibrationMap[providerPtr].TakeOver(calibrationCopyPtr);
			}
		}
	}

	BaseClass::AddItemsToScene(providerPtr, flags);

	if ((flags & iqt2d::IViewExtender::SF_DIRECT) != 0){
		if (m_paramsSetExtenderCompPtr.IsValid()){
			m_paramsSetExtenderCompPtr->AddItemsToScene(providerPtr, flags);
		}
	}
}


template <class UI>
void TSupplierGuiCompBase<UI>::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (		m_providerToCalibrationMap.contains(providerPtr) &&
				(*m_viewCalibrationModeAttrPtr >= VCM_ALWAYS)){
		iview::CCalibratedViewBase* viewPtr = dynamic_cast<iview::CCalibratedViewBase*>(providerPtr->GetView());
		const i2d::ICalibration2d* calibrationPtr = m_providerToCalibrationMap[providerPtr].GetPtr();

		if ((viewPtr != NULL) && (calibrationPtr != NULL) && (viewPtr->GetCalibration() == calibrationPtr)){
			viewPtr->SetDisplayCalibration(NULL);
		}
	}

	m_providerToCalibrationMap.remove(providerPtr);

	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// protected methods

template <class UI>
bool TSupplierGuiCompBase<UI>::AreParamsEditable() const
{
	return m_areParamsEditable;
}


template <class UI>
bool TSupplierGuiCompBase<UI>::IsLoadParamsSupported() const
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
bool TSupplierGuiCompBase<UI>::IsSaveParamsSupported() const
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
bool TSupplierGuiCompBase<UI>::LoadParams()
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
bool TSupplierGuiCompBase<UI>::SaveParams()
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
bool TSupplierGuiCompBase<UI>::DoTest()
{
	iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		return supplierPtr->GetWorkStatus() < iinsp::ISupplier::WS_ERROR;
	}

	return false;
}


template <class UI>
void TSupplierGuiCompBase<UI>::OnSupplierParamsChanged()
{
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class UI>
void TSupplierGuiCompBase<UI>::OnGuiModelAttached()
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

	BaseClass::SetStatusIcon(QIcon(":/Icons/StateUnknown"));
}


template <class UI>
void TSupplierGuiCompBase<UI>::OnGuiModelDetached()
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

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqt2d::TViewExtenderCompBase)

template <class UI>
void TSupplierGuiCompBase<UI>::CreateShapes(int /*sceneId*/, Shapes& /*result*/)
{
}


// reimplemented (imod::IObserver)

template <class UI>
void TSupplierGuiCompBase<UI>::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	if (!BaseClass::IsGuiCreated() || changeSet.ContainsExplicit(istd::IChangeable::CF_DESTROYING)){
		BaseClass::AfterUpdate(modelPtr, changeSet);

		return;
	}

	QString statusText = "";
	QIcon statusIcon = QIcon(":/Icons/StateUnknown");

	QString description;

	const iinsp::ISupplier* supplierPtr = BaseClass::GetObservedObject();
	if (supplierPtr != NULL){
		const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);
		int category = infoProviderPtr == NULL ? 
			istd::IInformationProvider::IC_INFO:
			infoProviderPtr->GetInformationCategory();

		int workStatus = supplierPtr->GetWorkStatus();

		switch (workStatus){
		case iinsp::ISupplier::WS_LOCKED:
			statusText = QObject::tr("Locked");
			break;

		case iinsp::ISupplier::WS_OK:
			switch (category){
				case istd::IInformationProvider::IC_WARNING:
					statusText = QObject::tr("Processing completed with warnings");
					statusIcon = QIcon(":/Icons/StateWarning");
					break;

				case istd::IInformationProvider::IC_ERROR:
					statusText = QObject::tr("Processing completed with errors");
					statusIcon = QIcon(":/Icons/StateInvalid");
					break;

				default:
					statusText = QObject::tr("Processing completed without errors");
					statusIcon = QIcon(":/Icons/StateOk");
					break;
				}
			break;

		case iinsp::ISupplier::WS_CANCELED:
			statusText = QObject::tr("Processing canceled by user");
			break;

		case iinsp::ISupplier::WS_ERROR:
			statusText = QObject::tr("Processing not possible");
			statusIcon = QIcon(":/Icons/StateInvalid");
			break;

		case iinsp::ISupplier::WS_CRITICAL:
			statusText = QObject::tr("Critical error occurred, application problem");
			statusIcon = QIcon(":/Icons/Error");
			break;

		default:
			break;
		}
	}

	if (*m_viewCalibrationModeAttrPtr >= VCM_ALWAYS){
		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(GetObservedObject());

		for (		ProviderToCalibrationMap::Iterator viewIter = m_providerToCalibrationMap.begin();
					viewIter != m_providerToCalibrationMap.end();
					++viewIter){
			const iqt2d::IViewProvider* providerPtr = viewIter.key();
			iview::CCalibratedViewBase* viewPtr = dynamic_cast<iview::CCalibratedViewBase*>(providerPtr->GetView());

			if ((calibrationProviderPtr != NULL) && (viewPtr != NULL)){
				istd::TDelPtr<i2d::ICalibration2d> calibrationCopyPtr;

				const i2d::ICalibration2d* calibrationPtr = calibrationProviderPtr->GetCalibration();
				if (calibrationPtr != NULL){
					const i2d::ICalibration2d* viewCalibrationPtr = viewIter.value().GetPtr();

					if ((viewCalibrationPtr != NULL) && viewCalibrationPtr->IsEqual(*calibrationPtr)){
						continue;
					}

					calibrationCopyPtr.SetCastedOrRemove(calibrationPtr->CloneMe());
				}

				viewPtr->SetDisplayCalibration(calibrationCopyPtr.GetPtr());

				viewIter.value().TakeOver(calibrationCopyPtr);
			}
		}
	}

	istd::CChangeNotifier visualStatusNotifier(&m_visualStatus);
	BaseClass::SetStatusIcon(statusIcon);
	BaseClass::SetStatusText(statusText);

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// public methods of embedded class ParamsObserver

template <class UI>
TSupplierGuiCompBase<UI>::ParamsObserver::ParamsObserver(TSupplierGuiCompBase* parentPtr)
	:m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)
template <class UI>
void TSupplierGuiCompBase<UI>::ParamsObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.OnSupplierParamsChanged();
}

} // namespace iqtinsp


#endif // !iqtinsp_TSupplierGuiCompBase_included


