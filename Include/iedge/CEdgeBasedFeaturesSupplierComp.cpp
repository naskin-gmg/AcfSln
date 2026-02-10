// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iedge/CEdgeBasedFeaturesSupplierComp.h>


// ACF includes
#include <ilog/CMessage.h>
#include <iprm/IParamsManager.h>
#include <iprm/TParamsPtr.h>

// ACF-Solutions includes
#include <iipr/CSearchFeature.h>
#include <iipr/ISearchParams.h>


namespace iedge
{


// public methods

CEdgeBasedFeaturesSupplierComp::CEdgeBasedFeaturesSupplierComp()
:	m_defaultInformationCategory(istd::IInformationProvider::IC_NONE)
{
}


// reimplemented (i2d::IMultiCalibrationProvider)

const iprm::IOptionsList* CEdgeBasedFeaturesSupplierComp::GetCalibrationSelectionContraints() const
{
	return NULL;
}


int CEdgeBasedFeaturesSupplierComp::GetCalibrationsCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return int(productPtr->second.size());
	}

	return 0;
}


const i2d::ICalibration2d* CEdgeBasedFeaturesSupplierComp::GetCalibration(int calibrationIndex) const
{
	Q_ASSERT(calibrationIndex >= 0);

	const ProductType* productPtr = GetWorkProduct();
	Q_ASSERT(productPtr != NULL);
	Q_ASSERT(calibrationIndex < int(productPtr->second.size()));

	return &productPtr->second.at(calibrationIndex);
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CEdgeBasedFeaturesSupplierComp::GetCalibration() const
{
	const ProductType* productPtr = GetWorkProduct();
	if ((productPtr != NULL) && !productPtr->second.empty()){
		return &productPtr->second[0];
	}

	return NULL;
}


// reimplemented (iipr::IFeaturesProvider)

int CEdgeBasedFeaturesSupplierComp::GetFeaturesCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first.GetFeaturesCount();
	}

	return 0;
}
	

const imeas::INumericValue& CEdgeBasedFeaturesSupplierComp::GetFeature(int index) const
{
	const ProductType* productPtr = GetWorkProduct();
	Q_ASSERT (productPtr != NULL);

	return productPtr->first.GetFeature(index);
}


// reimplemented (istd::IInformationProvider)

QDateTime CEdgeBasedFeaturesSupplierComp::GetInformationTimeStamp() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationTimeStamp();
	}

	return QDateTime::currentDateTime();
}


istd::IInformationProvider::InformationCategory CEdgeBasedFeaturesSupplierComp::GetInformationCategory() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationCategory();
	}

	return m_defaultInformationCategory;
}


int CEdgeBasedFeaturesSupplierComp::GetInformationId() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationId();
	}

	return -1;
}


QString CEdgeBasedFeaturesSupplierComp::GetInformationDescription() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationDescription();
	}

	return QString();
}


QString CEdgeBasedFeaturesSupplierComp::GetInformationSource() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationSource();
	}

	return QString();
}


int CEdgeBasedFeaturesSupplierComp::GetInformationFlags() const
{
	if (m_slaveInformationProviderCompPtr.IsValid()){
		return m_slaveInformationProviderCompPtr->GetInformationFlags();
	}

	return 0;
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

bool CEdgeBasedFeaturesSupplierComp::InitializeWork()
{
	if (m_searchProcessorCompPtr.IsValid()){
		if (!m_searchParamsManagerParamIdAttrPtr.IsValid()){
			m_searchProcessorCompPtr->InitProcessor(GetModelParametersSet());
		}

		return true;
	}

	return false;
}


iinsp::ISupplier::WorkStatus CEdgeBasedFeaturesSupplierComp::ProduceObject(ProductType& result) const
{
	if (		m_edgeLinesProviderCompPtr.IsValid() &&
				m_searchProcessorCompPtr.IsValid()){
		const CEdgeLineContainer* edgeLinesPtr = m_edgeLinesProviderCompPtr->GetEdgesContainer();
		if (edgeLinesPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			const iprm::IParamsManager* multiSearchParamsManagerPtr = NULL;
			
			if (m_searchParamsManagerParamIdAttrPtr.IsValid()){
				iprm::TParamsPtr<iprm::IParamsManager> paramsManagerPtr(paramsSetPtr, *m_searchParamsManagerParamIdAttrPtr);
				if (!paramsManagerPtr.IsValid()){
					SendErrorMessage(iproc::IProcessor::MI_BAD_PARAMS, "No inspection list found");

					return WS_FAILED;
				}

				multiSearchParamsManagerPtr = paramsManagerPtr.GetPtr();
			}

			Timer performanceTimer(this, "Search of features");

			if (multiSearchParamsManagerPtr != NULL){
				result.first.ResetFeatures();

				int searchCount = multiSearchParamsManagerPtr->GetParamsSetsCount();
				m_defaultInformationCategory = istd::IInformationProvider::IC_NONE;
				for (int searchIndex = 0; searchIndex < searchCount; searchIndex++){
					const iprm::IParamsSet* paramsPtr = multiSearchParamsManagerPtr->GetParamsSet(searchIndex);

					iipr::CFeaturesContainer localFeatures;
					int searchState = m_searchProcessorCompPtr->DoProcessing(
								paramsPtr,
								edgeLinesPtr,
								&localFeatures);

					if (searchState != iproc::IProcessor::TS_OK){
						ilog::CMessage* message = new ilog::CMessage(
									istd::IInformationProvider::IC_NONE,
									MI_SUPPLIER_RESULTS_STATUS,
									QObject::tr("Search not successful"),
									multiSearchParamsManagerPtr->GetParamsSetName(searchIndex));
						AddMessage(message);

						return WS_FAILED;
					}

					// logical backup status set to error if no models found
					const iipr::ISearchParams* searchParamsPtr = dynamic_cast<const iipr::ISearchParams*>(paramsPtr->GetParameter(*m_searchParamsIdAttrPtr));
					int nominalModelsCount = -1;
					if (searchParamsPtr != NULL){
						nominalModelsCount = searchParamsPtr->GetNominalModelsCount();
					}

					m_defaultInformationCategory = IC_INFO;
					QString searchResultText = QObject::tr("Search of geometric pattern successful");

					// check feature type and set correct ID
					int featuresCount = localFeatures.GetFeaturesCount();
					for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
						istd::TUniqueInterfacePtr<iipr::CSearchFeature> clonedFeaturePtr;
						clonedFeaturePtr.MoveCastedPtr(localFeatures.GetFeature(featureIndex).CloneMe());
						if (!clonedFeaturePtr.IsValid()){
							m_defaultInformationCategory = IC_CRITICAL;
							searchResultText = QObject::tr("Wrong result type");

							return WS_FAILED;
						}

						QString modelId = multiSearchParamsManagerPtr->GetParamsSetName(searchIndex) + "/" + clonedFeaturePtr->GetObjectId();
						clonedFeaturePtr->SetObjectId(modelId.toUtf8());

						if (		(m_defaultInformationCategory != istd::IInformationProvider::IC_ERROR) &&
									clonedFeaturePtr->IsNegativeModelEnabled()){
							searchResultText = QObject::tr("Not allowed model found: '%1'").arg(modelId);

							m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
						}

						result.first.AddFeature(clonedFeaturePtr.PopInterfacePtr());
					}

					if ((nominalModelsCount > 0) && (featuresCount < nominalModelsCount)){
						m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
						
						searchResultText = QObject::tr("Search model was not found");
					}

					ilog::CMessage* message = new ilog::CMessage(
								m_defaultInformationCategory,
								MI_SUPPLIER_RESULTS_STATUS,
								searchResultText,
								multiSearchParamsManagerPtr->GetParamsSetName(searchIndex));
					AddMessage(message);
				}
			}
			else{ // Single search
				int searchState = m_searchProcessorCompPtr->DoProcessing(
								paramsSetPtr,
								edgeLinesPtr,
								&result.first);

				if (searchState != iproc::IProcessor::TS_OK){
					ilog::CMessage* message = new ilog::CMessage(
								istd::IInformationProvider::IC_NONE,
								MI_SUPPLIER_RESULTS_STATUS,
								QObject::tr("Search not successful"),
								GetDiagnosticName());
					
					AddMessage(message);
					
					return WS_FAILED;
				}

				// check if certain amount of models was found
				int nominalModelsCount = -1;
				const iipr::ISearchParams* searchParamsPtr = dynamic_cast<const iipr::ISearchParams*>(paramsSetPtr->GetParameter(*m_searchParamsIdAttrPtr));
				if (searchParamsPtr != NULL){
					nominalModelsCount = searchParamsPtr->GetNominalModelsCount();
				}

				m_defaultInformationCategory = IC_INFO;
				QString searchResultText = QObject::tr("Search of geometric pattern successful");

				int featuresCount = result.first.GetFeaturesCount();
				for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
					iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<iipr::CSearchFeature*>(&result.first.GetFeatureRef(featureIndex));
					if (searchFeaturePtr == NULL){
						m_defaultInformationCategory = IC_CRITICAL;
						searchResultText = QObject::tr("Wrong result type");

						return WS_FAILED;
					}

					QString modelId = searchFeaturePtr->GetObjectId();

					if (m_defaultInformationCategory != istd::IInformationProvider::IC_ERROR && searchFeaturePtr->IsNegativeModelEnabled()){
						searchResultText = QObject::tr("Not allowed model found: '%1'").arg(modelId);

						m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
					}
				}

				if ((nominalModelsCount > 0) && (featuresCount < nominalModelsCount)){
					m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
					searchResultText = QObject::tr("Search model was not found");
				}

				QString sourceName = GetDiagnosticName();
				if (sourceName.isEmpty()){
					sourceName = "SearchResult";
				}

				ilog::CMessage* message = new ilog::CMessage(
							m_defaultInformationCategory,
							MI_SUPPLIER_RESULTS_STATUS,
							searchResultText,
							sourceName);
				AddMessage(message);
			}

			// Update calibration list
			int featuresCount = result.first.GetFeaturesCount();
			result.second.resize(featuresCount);
			for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
				i2d::CAffineTransformation2d& transform = result.second[featureIndex];

				const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(&result.first.GetFeature(featureIndex));
				Q_ASSERT(searchFeaturePtr != NULL);

				transform.Reset(searchFeaturePtr->GetPosition(), -searchFeaturePtr->GetAngle(), searchFeaturePtr->GetScale());

				if (m_calibrationProviderCompPtr.IsValid()){
					const i2d::ICalibration2d* calibrationPtr = m_calibrationProviderCompPtr->GetCalibration();
					if (calibrationPtr != NULL){
						istd::TUniqueInterfacePtr<i2d::ICalibration2d> combinedPtr = calibrationPtr->CreateCombinedCalibration(transform);
						if (combinedPtr.IsValid()){
							combinedPtr->GetLocalTransform(i2d::CVector2d(0, 0), transform.GetTransformationRef());
						}
					}
				}
			}

			return WS_OK;
		}
	}

	return WS_FAILED;
}


// reimplemented (icomp::CComponentBase)

void CEdgeBasedFeaturesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_calibrationProviderModelCompPtr.GetPtr(), m_calibrationSupplierCompPtr.GetPtr());
	}

	if (m_edgeLinesProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_edgeLinesProviderModelCompPtr.GetPtr(), m_edgeLinesSupplierCompPtr.GetPtr());
	}

	// Force initialization to avoid crash in multithreading
	m_searchProcessorCompPtr.EnsureInitialized();
}


void CEdgeBasedFeaturesSupplierComp::OnComponentDestroyed()
{
	if (m_edgeLinesProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_edgeLinesProviderModelCompPtr.GetPtr());
	}

	if (m_calibrationProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_calibrationProviderModelCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iedge


