#include "iipr/CSearchBasedFeaturesSupplierComp.h"


// ACF includes
#include "iprm/IParamsManager.h"
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "iipr/CSearchFeature.h"


namespace iipr
{


// public methods

// reimplemented (i2d::IMultiCalibrationProvider)

const iprm::IOptionsList* CSearchBasedFeaturesSupplierComp::GetCalibrationSelectionContraints() const
{
	return NULL;
}


int CSearchBasedFeaturesSupplierComp::GetCalibrationsCount() const
{
	return m_transformationList.count();
}


const i2d::ICalibration2d* CSearchBasedFeaturesSupplierComp::GetCalibration(int calibrationIndex) const
{
	Q_ASSERT(calibrationIndex >= 0);
	Q_ASSERT(calibrationIndex < m_transformationList.count());

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
	Q_ASSERT (containerPtr != NULL);

	return containerPtr->GetNumericValue(index);
}


// reimplemented (istd::IInformationProvider)

QDateTime CSearchBasedFeaturesSupplierComp::GetInformationTimeStamp() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationTimeStamp();
	}

	return QDateTime::currentDateTime();
}


istd::IInformationProvider::InformationCategory CSearchBasedFeaturesSupplierComp::GetInformationCategory() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationCategory();
	}

	return istd::IInformationProvider::IC_NONE;
}


int CSearchBasedFeaturesSupplierComp::GetInformationId() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationId();
	}

	return -1;
}


QString CSearchBasedFeaturesSupplierComp::GetInformationDescription() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationDescription();
	}

	return "";
}


QString CSearchBasedFeaturesSupplierComp::GetInformationSource() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationSource();
	}

	return "";
}


int CSearchBasedFeaturesSupplierComp::GetInformationFlags() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationFlags();
	}

	return 0;
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

bool CSearchBasedFeaturesSupplierComp::InitializeWork()
{
	if (m_searchProcessorCompPtr.IsValid()){
		m_searchProcessorCompPtr->InitProcessor(GetModelParametersSet());

		return true;
	}

	return false;
}


int CSearchBasedFeaturesSupplierComp::ProduceObject(CFeaturesContainer& result) const
{
	m_transformationList.clear();

	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_searchProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			const iprm::IParamsManager* multiSearchParamsManagerPtr = NULL;
			
			if (m_searchParamsManagerParamIdAttrPtr.IsValid()){
				iprm::TParamsPtr<iprm::IParamsManager> paramsManagerPtr(paramsSetPtr, *m_searchParamsManagerParamIdAttrPtr);
				if (!paramsManagerPtr.IsValid()){
					SendErrorMessage(iproc::IProcessor::MI_BAD_PARAMS, "No inspection list found");

					return WS_ERROR;
				}

				multiSearchParamsManagerPtr = paramsManagerPtr.GetPtr();
			}

			Timer performanceTimer(this, "Search of features");

			if (multiSearchParamsManagerPtr != NULL){
				result.ResetFeatures();

				int searchCount = multiSearchParamsManagerPtr->GetParamsSetsCount();
				for (int searchIndex = 0; searchIndex < searchCount; searchIndex++){
					const iprm::IParamsSet* paramsPtr = multiSearchParamsManagerPtr->GetParamsSet(searchIndex);

					CFeaturesContainer searchResults;
					int searchState = m_searchProcessorCompPtr->DoProcessing(
						paramsPtr,
						bitmapPtr,
						&searchResults);

					if (searchState != iproc::IProcessor::TS_OK){
						return WS_ERROR;
					}

					int featuresCount = searchResults.GetValuesCount();
					for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
						istd::IChangeable* featurePtr = searchResults.GetNumericValue(featureIndex).CloneMe();
						if (featurePtr == NULL){
							return WS_CRITICAL;
						}

						imeas::INumericValue* valuePtr = dynamic_cast<imeas::INumericValue*>(featurePtr);
						if (valuePtr == NULL){
							return WS_CRITICAL;
						}

						result.AddFeature(valuePtr);
					}
				}
			}
			else{
				int searchState = m_searchProcessorCompPtr->DoProcessing(
								paramsSetPtr,
								bitmapPtr,
								&result);

				if (searchState != iproc::IProcessor::TS_OK){
					return WS_ERROR;
				}
			}

			// Update calibration list:
			int featuresCount = result.GetValuesCount();
			for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
				i2d::CAffineTransformation2d transform;

				const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(&result.GetNumericValue(featureIndex));
				Q_ASSERT(searchFeaturePtr != NULL);

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


