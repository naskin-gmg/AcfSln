#include <iipr/CSelectProcessedBitmapSupplierComp.h>

// ACF includes
#include <ilog/CMessage.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IParamsManager.h>
#include <iprm/TParamsPtr.h>


namespace iipr
{


// public methods


// reimplemented (iinsp::TSupplierCompWrap)

iinsp::ISupplier::WorkStatus CSelectProcessedBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	const iimg::IBitmapProvider* bitmapProviderPtr = GetBitmapProvider();
	if (bitmapProviderPtr == nullptr){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QT_TR_NOOP("No input bitmap provider"), "BitmapProcessing"));
		return WS_FAILED;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_FAILED;
	}
	Q_ASSERT(result.IsValid());

	const iimg::IBitmap* bitmapPtr = bitmapProviderPtr->GetBitmap();
	if (bitmapPtr == nullptr){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QT_TR_NOOP("No input image"), "BitmapProcessing"));

		return WS_FAILED;
	}

	iimg::IBitmapUniquePtr tempBitmapPtr(CreateBitmap());
	tempBitmapPtr->CopyFrom(*const_cast<iimg::IBitmap*>(bitmapPtr));

	Timer performanceTimer(this, "Selectable bitmap processing");

	int regionsCount = 0;
	const iprm::IParamsManager* paramsManagerPtr = nullptr;

	const iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();
	if (paramsSetPtr != nullptr){
		const iprm::TParamsPtr<iprm::IParamsManager> paramsManagerParamPtr(paramsSetPtr, *m_algorithmManagerIdAttrPtr);
		if (paramsManagerParamPtr != nullptr){
			regionsCount = paramsManagerParamPtr->GetParamsSetsCount();
			paramsManagerPtr = paramsManagerParamPtr.GetPtr();
		}
	}

	if (!regionsCount){
		// Generally, this one should be refactored
		AddMessage(ilog::CMessage::IC_WARNING, QT_TR_NOOP("No processing regions defined"));

		// drop old image, replace with original imput
		result->CopyFrom(*bitmapPtr);

		// we deliberately wont fail here - with "ok" we wanna see input image instead of /\ ...
		return WS_OK;
	}

	iproc::IProcessor::TaskState status = iproc::IProcessor::TS_NONE;

	for (int regionIndex = 0; regionIndex < regionsCount; ++regionIndex){

		iproc::IProcessor* imageProcessorPtr = nullptr;
		const iprm::IParamsSet* currentParamsPtr = nullptr;

		if (paramsManagerPtr != nullptr){
			currentParamsPtr = paramsManagerPtr->GetParamsSet(regionIndex);
			if (currentParamsPtr != nullptr){
				QByteArray paramsSetId = currentParamsPtr->GetFactoryId();
				const int processorIdx = GetProcessorIdx(paramsSetId);
				if (processorIdx > -1){
					imageProcessorPtr = m_imageProcessorCompPtr[processorIdx];
				}
			}
		}

		if (imageProcessorPtr == nullptr) {
			AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QT_TR_NOOP("Image processor is not accessible"), "BitmapProcessing"));
			return WS_FAILED;
		}
		if (currentParamsPtr == nullptr) {
			AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QT_TR_NOOP("Image processor parameters are not accessible"), "BitmapProcessing"));
			return WS_FAILED;
		}

		status = std::max(status, imageProcessorPtr->DoProcessing(currentParamsPtr, tempBitmapPtr.GetPtr(), result.GetPtr()));

		tempBitmapPtr->CopyFrom(*result.GetPtr());
	}

	switch (status){
	case iproc::IProcessor::TS_OK:
		return WS_OK;

	case iproc::IProcessor::TS_CANCELED:
		return WS_CANCELED;

	default:
		return WS_FAILED;
	}

	// should not get here
	return WS_FAILED;
}


// reimplemented (icomp::CComponentBase)

void CSelectProcessedBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_processorNamesAttrPtr.IsValid()){
		const int count = m_processorNamesAttrPtr.GetCount();
		for (int i = 0; i < count; ++i){
			m_processorNames.InsertOption(QString(m_processorNamesAttrPtr[i]), QByteArray::number(i));
		}
	}

}


void CSelectProcessedBitmapSupplierComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();
}


// protected methods

const int CSelectProcessedBitmapSupplierComp::GetProcessorIdx(const QByteArray paramId) const
{
	if (m_processorIdsAttrPtr.IsValid()){
		const int procCount = m_processorIdsAttrPtr.GetCount();
		for (int idx = 0; idx < procCount; ++idx){
			if (m_processorIdsAttrPtr[idx] == paramId)
				return idx;
		}
	}
	return -1;
}


const iprm::IParamsSet* CSelectProcessedBitmapSupplierComp::GetParametersSelected() const
{
	const iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();
	if (paramsSetPtr != nullptr) {
		const iprm::TParamsPtr<iprm::IParamsManager> paramsManagerPtr(paramsSetPtr, *m_algorithmManagerIdAttrPtr);
		if (paramsManagerPtr != nullptr) {
			const int index = paramsManagerPtr->GetSelectedOptionIndex();
			if ((index > -1) && (index < paramsManagerPtr->GetParamsSetsCount()))
				return paramsManagerPtr->GetParamsSet(index);
		}
	}

	return nullptr;
}


// reimplemented iipr::CProcessedBitmapSupplierBase
iproc::IProcessor* CSelectProcessedBitmapSupplierComp::GetImageProcessor() const
{
	const iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();
	if (paramsSetPtr != nullptr) {
		const iprm::TParamsPtr<iprm::IParamsManager> paramsManagerPtr(paramsSetPtr, *m_algorithmManagerIdAttrPtr);
		if (paramsManagerPtr != nullptr) {
			const int index = paramsManagerPtr->GetSelectedOptionIndex();
			if (m_imageProcessorCompPtr.IsValid()) {
				const int count = m_imageProcessorCompPtr.GetCount();
				if ((index > -1) && (index < count)) {
					const iprm::IParamsSet* currentParamsPtr = paramsManagerPtr->GetParamsSet(index);
					const QByteArray type = currentParamsPtr->GetFactoryId();
					return m_imageProcessorCompPtr[index];
				}
			}
		}
	}

	return nullptr;
}


} // namespace iipr


