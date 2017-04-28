#include <iipr/CSearchBasedFeaturesSupplierComp.h>


// ACF includes
#include <iprm/IParamsManager.h>
#include <iprm/TParamsPtr.h>

// ACF-Solutions includes
#include <iipr/CSearchFeature.h>
#include <iipr/ISearchParams.h>


namespace iipr
{


// public methods

CSearchBasedFeaturesSupplierComp::CSearchBasedFeaturesSupplierComp()
	: m_defaultInformationCategory(istd::IInformationProvider::IC_NONE)
{
	m_calibrationList.SetParentPtr(this);
}


// reimplemented (i2d::IMultiCalibrationProvider)

const iprm::IOptionsList* CSearchBasedFeaturesSupplierComp::GetCalibrationSelectionContraints() const
{
	return &m_calibrationList;
}


int CSearchBasedFeaturesSupplierComp::GetCalibrationsCount() const
{
	return m_calibrations.count();
}


const i2d::ICalibration2d* CSearchBasedFeaturesSupplierComp::GetCalibration(int calibrationIndex) const
{
	Q_ASSERT(calibrationIndex >= 0);
	Q_ASSERT(calibrationIndex < m_calibrations.count());

	return &m_calibrations.at(calibrationIndex).calibration;
}


// reimplemented (iipr::IFeaturesProvider)

int CSearchBasedFeaturesSupplierComp::GetFeaturesCount() const
{
	const CFeaturesContainer* containerPtr = GetWorkProduct();
	if (containerPtr != NULL){
		return containerPtr->GetFeaturesCount();
	}

	return 0;
}
	

const imeas::INumericValue& CSearchBasedFeaturesSupplierComp::GetFeature(int index) const
{
	const CFeaturesContainer* containerPtr = GetWorkProduct();
	Q_ASSERT (containerPtr != NULL);

	return containerPtr->GetFeature(index);
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

	return m_defaultInformationCategory;
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

// reimplemented (iinsp::TSupplierCompWrap)

bool CSearchBasedFeaturesSupplierComp::InitializeWork()
{
	if (m_searchProcessorCompPtr.IsValid()){
		if (!m_searchParamsManagerParamIdAttrPtr.IsValid()){
			m_searchProcessorCompPtr->InitProcessor(GetModelParametersSet());
		}

		return true;
	}

	return false;
}


int CSearchBasedFeaturesSupplierComp::ProduceObject(CFeaturesContainer& result) const
{
	m_calibrations.clear();

	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_searchProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr == NULL){
			AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("No input image"), "FeatureSearch"));

			return WS_ERROR;
		}

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
			m_defaultInformationCategory = istd::IInformationProvider::IC_NONE;
			for (int searchIndex = 0; searchIndex < searchCount; searchIndex++){
				const iprm::IParamsSet* paramsPtr = multiSearchParamsManagerPtr->GetParamsSet(searchIndex);

				m_searchProcessorCompPtr->InitProcessor(paramsPtr);

				CFeaturesContainer searchResults;
				int searchState = m_searchProcessorCompPtr->DoProcessing(
					paramsPtr,
					bitmapPtr,
					&searchResults);

				if (searchState != iproc::IProcessor::TS_OK){
					ilog::CMessage* message = new ilog::CMessage(
								istd::IInformationProvider::IC_NONE,
								MI_SUPPLIER_RESULTS_STATUS,
								QObject::tr("Search not successfull"),
								multiSearchParamsManagerPtr->GetParamsSetName(searchIndex));

					AddMessage(message);

					return WS_ERROR;
				}

				int featuresCount = searchResults.GetFeaturesCount();

				// logical backup status set to error if no models found
				const iipr::ISearchParams* searchParamsPtr = dynamic_cast<const iipr::ISearchParams*>(paramsPtr->GetParameter(*m_searchParamsIdAttrPtr));
				int nominalModelsCount = 0;
				if (searchParamsPtr != NULL){
					nominalModelsCount = searchParamsPtr->GetNominalModelsCount();
				}

				m_defaultInformationCategory = (featuresCount < nominalModelsCount) ? istd::IInformationProvider::IC_ERROR : istd::IInformationProvider::IC_INFO;
				QString searchResultText = (m_defaultInformationCategory == istd::IInformationProvider::IC_INFO) ? 
					"Search model was found" : 
					"Search model was not found"; 

				ilog::CMessage* message = new ilog::CMessage(
							m_defaultInformationCategory,
							MI_SUPPLIER_RESULTS_STATUS,
							searchResultText,
							multiSearchParamsManagerPtr->GetParamsSetName(searchIndex));

				AddMessage(message);

				if (m_defaultInformationCategory != istd::IInformationProvider::IC_ERROR && featuresCount < nominalModelsCount){
					m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
				}

				for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
					const iipr::CObjectFeature* objectFeaturePtr = dynamic_cast<const iipr::CObjectFeature*>(&searchResults.GetFeature(featureIndex));
					if (objectFeaturePtr == NULL){
						return WS_CRITICAL;

					}
					const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(objectFeaturePtr);
					if (searchFeaturePtr == NULL){
						return WS_CRITICAL;
					}

					if (		m_defaultInformationCategory != istd::IInformationProvider::IC_ERROR && 
								((searchFeaturePtr != NULL) && searchFeaturePtr->IsNegativeModelEnabled())){
						m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
					}

					QByteArray featureId = objectFeaturePtr->GetObjectId();
					QByteArray objectId;

					if (!featureId.isEmpty()){
						objectId = multiSearchParamsManagerPtr->GetParamsSetName(searchIndex).toUtf8() + "/" + objectFeaturePtr->GetObjectId();
					}
					else{
						const iprm::IOptionsList* paramListPtr = multiSearchParamsManagerPtr->GetSelectionConstraints();
						if (paramListPtr != NULL){
							objectId = paramListPtr->GetOptionId(searchIndex);
						}
					}

					const_cast<iipr::CObjectFeature*>(objectFeaturePtr)->SetObjectId(objectId);

					istd::IChangeable* featurePtr = objectFeaturePtr->CloneMe();
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
		else{ // Single search
			int searchState = m_searchProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&result);

			if (searchState != iproc::IProcessor::TS_OK){
				ilog::CMessage* message = new ilog::CMessage(
							istd::IInformationProvider::IC_NONE,
							MI_SUPPLIER_RESULTS_STATUS,
							QObject::tr("Search not successfull"),
							GetDiagnosticName());
					
				AddMessage(message);

				return WS_ERROR;
			}

			// check if certain amount of models was found
			int modelsCount = result.GetFeaturesCount();
			int nominalModelsCount = -1;

			const iipr::ISearchParams* searchParamsPtr = dynamic_cast<const iipr::ISearchParams*>(paramsSetPtr->GetParameter(*m_searchParamsIdAttrPtr));
			if (searchParamsPtr != NULL){
				nominalModelsCount = searchParamsPtr->GetNominalModelsCount();
			}

			if (nominalModelsCount > 0 && modelsCount < nominalModelsCount){
				m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
			}
			else{
				m_defaultInformationCategory = istd::IInformationProvider::IC_INFO;
			}

			for (int featureIndex = 0; featureIndex < modelsCount; featureIndex++){
				const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(&result.GetFeature(featureIndex));
				if ((searchFeaturePtr != NULL) && (m_defaultInformationCategory != istd::IInformationProvider::IC_ERROR && searchFeaturePtr->IsNegativeModelEnabled())){
					m_defaultInformationCategory = istd::IInformationProvider::IC_ERROR;
				}
			}				

			QString searchResultText = (m_defaultInformationCategory == istd::IInformationProvider::IC_INFO)? 
						QObject::tr("Search model was found"): 
						QObject::tr("Search model was not found"); 

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
		int featuresCount = result.GetFeaturesCount();
		for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
			CalibrationInfo calibrationInfo;

			const iipr::CObjectFeature* objectFeaturePtr = dynamic_cast<const iipr::CObjectFeature*>(&result.GetFeature(featureIndex));
			Q_ASSERT(objectFeaturePtr != NULL);

			calibrationInfo.calibration.Reset(objectFeaturePtr->GetPosition(), -objectFeaturePtr->GetAngle(), objectFeaturePtr->GetScale());
			calibrationInfo.calibrationId = objectFeaturePtr->GetObjectId();

			m_calibrations.push_back(calibrationInfo);
		}

		return WS_OK;
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

	// Force initialization to avoid crash in multithreading
	m_searchProcessorCompPtr.EnsureInitialized();
}


void CSearchBasedFeaturesSupplierComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}
}


// public methods of the embedded class CalibrationList

CSearchBasedFeaturesSupplierComp::CalibrationList::CalibrationList()
	:m_parentPtr(NULL)
{
}


void CSearchBasedFeaturesSupplierComp::CalibrationList::SetParentPtr(CSearchBasedFeaturesSupplierComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (iprm::IOptionsList)

int CSearchBasedFeaturesSupplierComp::CalibrationList::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CSearchBasedFeaturesSupplierComp::CalibrationList::GetOptionsCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_calibrations.count();
}


QString CSearchBasedFeaturesSupplierComp::CalibrationList::GetOptionName(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_calibrations[index].calibrationName;
}


QString CSearchBasedFeaturesSupplierComp::CalibrationList::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CSearchBasedFeaturesSupplierComp::CalibrationList::GetOptionId(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_calibrations[index].calibrationId;
}


bool CSearchBasedFeaturesSupplierComp::CalibrationList::IsOptionEnabled(int /*index*/) const
{
	return true;
}


} // namespace iipr


