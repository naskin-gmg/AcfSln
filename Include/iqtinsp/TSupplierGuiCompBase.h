#ifndef iqtinsp_TSupplierGuiCompBase_included
#define iqtinsp_TSupplierGuiCompBase_included


// ACF includes
#include <i2d/ICalibrationProvider.h>
#include <iview/CCalibratedViewBase.h>
#include <iqt2d/TViewExtenderCompBase.h>

// ACF-Solutions includes
#include <iqtinsp/TCommonSupplierGuiCompBase.h>


namespace iqtinsp
{


template <class UI>
class TSupplierGuiCompBase:
			public iqt2d::TViewExtenderCompBase<
						iqtinsp::TCommonSupplierGuiCompBase<UI>>
{
public:
	typedef iqt2d::TViewExtenderCompBase<
				iqtinsp::TCommonSupplierGuiCompBase<UI>> BaseClass;

	I_BEGIN_BASE_COMPONENT(TSupplierGuiCompBase);
		I_ASSIGN(m_paramsSetExtenderCompPtr, "ParamsSceneExtender", "Scene extender for the parameter editors", false, "ParamsSceneExtender");
		I_ASSIGN(m_viewCalibrationModeAttrPtr, "ViewCalibrationMode", "Control when calibration from supplier will be set to view\n\t0 - never use calibration\n\t1 - only indirect (used as slave)\n\t2 - allways set calibration if available", true, 0);
	I_END_COMPONENT;

	TSupplierGuiCompBase();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

protected:
	/**
		Extract calibration provider from the observed object.
	*/
	virtual const i2d::ICalibrationProvider* ExtractCalibrationProvider() const;

	// reimplemented (TCommonSupplierGuiCompBase)
	virtual void BeforeSupplierGuiUpdated();
		
	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

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

	I_REF(iqt2d::IViewExtender, m_paramsSetExtenderCompPtr);

	I_ATTR(int, m_viewCalibrationModeAttrPtr);

	typedef QMap<const iqt2d::IViewProvider*, istd::TDelPtr<i2d::ICalibration2d> > ProviderToCalibrationMap;
	ProviderToCalibrationMap m_providerToCalibrationMap;
};


// public methods

template <class UI>
TSupplierGuiCompBase<UI>::TSupplierGuiCompBase()
{
}


// reimplemented (iqt2d::IViewExtender)

template <class UI>
void TSupplierGuiCompBase<UI>::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	if (*m_viewCalibrationModeAttrPtr > VCM_NONE){
		if ((*m_viewCalibrationModeAttrPtr >= VCM_ALWAYS) || ((flags & iqt2d::IViewExtender::SF_DIRECT) == 0)){
			const i2d::ICalibrationProvider* calibrationProviderPtr = ExtractCalibrationProvider();

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
const i2d::ICalibrationProvider* TSupplierGuiCompBase<UI>::ExtractCalibrationProvider() const
{
	const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(GetObservedObject());
	if (calibrationProviderPtr == NULL){
		calibrationProviderPtr = CompCastPtr<const i2d::ICalibrationProvider>(GetObservedObject());
	}

	return calibrationProviderPtr;
}


// reimplemented (TCommonSupplierGuiCompBase)

template <class UI>
void TSupplierGuiCompBase<UI>::BeforeSupplierGuiUpdated()
{
	if (*m_viewCalibrationModeAttrPtr >= VCM_ALWAYS){
		const i2d::ICalibrationProvider* calibrationProviderPtr = ExtractCalibrationProvider();

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
}


// reimplemented (iqt2d::TViewExtenderCompBase)

template <class UI>
void TSupplierGuiCompBase<UI>::CreateShapes(int /*sceneId*/, Shapes& /*result*/)
{
}


} // namespace iqtinsp


#endif // !iqtinsp_TSupplierGuiCompBase_included


