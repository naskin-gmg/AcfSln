#include "iipr/CSearchBasedFeaturesSupplierComp.h"


// ACF-Solutions includes
#include "iipr/CSearchFeature.h"


namespace iipr
{


// public methods

// reimplemented (i2d::IMultiCalibrationProvider)

const iprm::ISelectionConstraints* CSearchBasedFeaturesSupplierComp::GetCalibrationSelectionContraints() const
{
	return NULL;
}


int CSearchBasedFeaturesSupplierComp::GetCalibrationsCount() const
{
	return m_transformationList.count();
}


const i2d::ICalibration2d* CSearchBasedFeaturesSupplierComp::GetCalibration(int calibrationIndex) const
{
	I_ASSERT(calibrationIndex >= 0);
	I_ASSERT(calibrationIndex < m_transformationList.count());

	return &m_transformationList.at(calibrationIndex);
}


// reimplemented (imeas::INumericValueProvider)

int CSearchBasedFeaturesSupplierComp::GetValuesCount() const
{
	const CFeaturesContainer* containerPtr = GetWorkProduct();
	if (containerPtr != NULL){
		return containerPtr->GetValuesCount();
	}

	return 0;
}
	

const imeas::INumericValue& CSearchBasedFeaturesSupplierComp::GetNumericValue(int index) const
{
	const CFeaturesContainer* containerPtr = GetWorkProduct();
	I_ASSERT (containerPtr != NULL);

	return containerPtr->GetNumericValue(index);
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSearchBasedFeaturesSupplierComp::ProduceObject(CFeaturesContainer& result) const
{
	m_transformationList.clear();

	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_searchProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			Timer performanceTimer(this, "Search of features");

			int searchState = m_searchProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&result);

			if (searchState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			// Update calibration list:
			int featuresCount = result.GetValuesCount();
			for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
				i2d::CAffineTransformation2d transform;

				const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(&result.GetNumericValue(featureIndex));
				I_ASSERT(searchFeaturePtr != NULL);

				transform.Reset(searchFeaturePtr->GetPosition(), -searchFeaturePtr->GetAngle(), searchFeaturePtr->GetScale());

				m_transformationList.push_back(transform);
			}

			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CSearchBasedFeaturesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}
}


void CSearchBasedFeaturesSupplierComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}
}


} // namespace iipr


