#include <iblob/CBlobProcessorCompBase.h>


// ACF includes
#include <iprm/TParamsPtr.h>


namespace iblob
{


// public methods

// reimplemented (iipr::IImageToFeatureProcessor)

int CBlobProcessorCompBase::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& results,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	if (paramsPtr == NULL){
		return TS_INVALID;
	}

	iprm::TParamsPtr<iblob::IBlobFilterParams> filterParamsPtr(paramsPtr, *m_filterParamsIdAttrPtr);

	bool retVal = CalculateBlobs(paramsPtr, filterParamsPtr.GetPtr(), image, results)? TS_OK: TS_INVALID;
	if (!retVal){
		return TS_INVALID;
	}

	return retVal ? TS_OK: TS_INVALID;
}


// reimplemented (iproc::IProcessor)

int CBlobProcessorCompBase::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	if (outputPtr == NULL){
		return TS_OK;
	}

	iipr::IFeaturesConsumer* outputConsumerPtr = dynamic_cast<iipr::IFeaturesConsumer*>(outputPtr);
	if (outputConsumerPtr == NULL){
		return TS_INVALID;
	}

	return DoExtractFeatures(paramsPtr, *inputBitmapPtr, *outputConsumerPtr, progressManagerPtr);
}


// protected static methods

bool CBlobProcessorCompBase::IsValueAcceptedByFilter(const iblob::IBlobFilterParams::Filter& filter, double value)
{
	bool isGreater = value > filter.valueRange.GetMinValue();
	bool isLess = value > filter.valueRange.GetMinValue();
	bool isEqual = qFuzzyCompare(value, filter.valueRange.GetMinValue());
	bool isGreaterEqual = isGreater || isEqual;
	bool isLessEqual = isLess || isEqual;

	switch (filter.condition){
	case iblob::IBlobFilterParams::FC_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isEqual : !isEqual;

	case iblob::IBlobFilterParams::FC_NOT_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? !isEqual : isEqual;

	case iblob::IBlobFilterParams::FC_BETWEEN:
		{
			bool valueInRange = filter.valueRange.Contains(value);
			return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? valueInRange : !valueInRange;
		}

	case iblob::IBlobFilterParams::FC_OUTSIDE:
		{
			bool valueInRange = filter.valueRange.Contains(value);
			return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? !valueInRange : valueInRange;
		}

	case iblob::IBlobFilterParams::FC_GREATER:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isGreater : !isGreater;

	case iblob::IBlobFilterParams::FC_GREATER_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isGreaterEqual : !isGreaterEqual;

	case iblob::IBlobFilterParams::FC_LESS:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isLess : !isLess;

	case iblob::IBlobFilterParams::FC_LESS_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isLessEqual : !isLessEqual;

	default:
		return true;
	}
}


} // namespace iblob


