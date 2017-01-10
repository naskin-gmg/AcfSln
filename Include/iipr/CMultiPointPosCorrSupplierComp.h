#pragma once


// ACF includes
#include <istd/TDelPtr.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <iimg/IBitmapProvider.h>

// ACF-Solutions includes
#include <iipr/IImageToFeatureProcessor.h>
#include <iinsp/TSupplierCompWrap.h>


namespace iipr
{


/*
	Supplier providing correction of calibration based on set of offset points.
*/

class CMultiPointPosCorrSupplierComp:
			public iinsp::TSupplierCompWrap< istd::TDelPtr<const i2d::ICalibration2d> >,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap< istd::TDelPtr<const i2d::ICalibration2d> > BaseClass;
	
	I_BEGIN_COMPONENT(CMultiPointPosCorrSupplierComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_pointParamsManagerIdAttrPtr, "PointParamsManagerId", "Id of single point parameter manager in ParamsSet (iprm::IParamsManager)", true, "PointParamsManager");
		I_ASSIGN(m_destPositionIdAttrPtr, "DestPositionId", "Id of an adhesive polyline in a parameter manager (i2d::CPosition2d)", true, "DestPosition");
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide input image", true, "BitmapProvider");
		I_ASSIGN_TO(m_calibrationProviderCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_MULTI_0(m_featureProcessorsCompPtr, "FeatureProcessors", "List of processor creating feature points", true);
		I_ASSIGN_MULTI_0(m_paramsSetTypeIdsAttrPtr, "ParamTypeIds", "The list of type IDs corresponding to processor list, if not set simple first processor will be used", false);
	I_END_COMPONENT;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	/**
		Get processor correcponding to some parameter set.
	*/
	iipr::IImageToFeatureProcessor* GetCompatibleProcessor(const iprm::IParamsSet* paramsSetPtr) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(QByteArray, m_pointParamsManagerIdAttrPtr);
	I_ATTR(QByteArray, m_destPositionIdAttrPtr);
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_MULTIREF(iipr::IImageToFeatureProcessor, m_featureProcessorsCompPtr);
	I_MULTIATTR(QByteArray, m_paramsSetTypeIdsAttrPtr);
};


} // namespace iipr


