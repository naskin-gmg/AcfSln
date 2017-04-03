#include <iblob/CBlobProcessorCompBase.h>


// ACF includes
#include <iprm/TParamsPtr.h>


namespace iblob
{


static QByteArray areaFeatureId("Area");
static QByteArray perimeterFeatureId("Perimeter");
static QByteArray circularityFeatureId("Circularity");


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

	iprm::TParamsPtr<iblob::IBlobFilterParams> filterParamsPtr(paramsPtr, m_filterParamsIdAttrPtr, m_defaultFilterParamsCompPtr);
	iprm::TParamsPtr<i2d::IObject2d> aoiPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr);

	bool retVal = CalculateBlobs(paramsPtr, filterParamsPtr.GetPtr(), aoiPtr.GetPtr(), image, results)? TS_OK: TS_INVALID;
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

bool CBlobProcessorCompBase::IsBlobAcceptedByFilter(const iblob::IBlobFilterParams& filterParams, double area, double perimeter, double circularity)
{
	if (filterParams.IsFiltersEnabled()){
		int filtersCount = filterParams.GetFiltersCount();

		for (int filterIndex = 0; filterIndex < filtersCount; ++filterIndex){
			const iblob::IBlobFilterParams::Filter& filter = filterParams.GetFilterAt(filterIndex);

			if (filter.featureId == areaFeatureId){
				if (!IsValueAcceptedByFilter(filter, area)){
					return false;
				}
			}

			if (filter.featureId == perimeterFeatureId){
				if (!IsValueAcceptedByFilter(filter, perimeter)){
					return false;
				}
			}

			if (filter.featureId == circularityFeatureId){
				if (!IsValueAcceptedByFilter(filter, circularity)){
					return false;
				}
			}
		}
	}

	return true;
}


// provate static methods

bool CBlobProcessorCompBase::IsValueAcceptedByFilter(const iblob::IBlobFilterParams::Filter& filter, double value)
{
	bool isGreater = value > filter.valueRange.GetMinValue();
	bool isLess = value > filter.valueRange.GetMinValue();
	bool isEqual = qFuzzyCompare(value, filter.valueRange.GetMinValue());
	bool isGreaterEqual = isGreater || isEqual;
	bool isLessEqual = isLess || isEqual;

	switch (filter.condition){
	case iblob::IBlobFilterParams::FC_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? isEqual: !isEqual;

	case iblob::IBlobFilterParams::FC_NOT_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? !isEqual: isEqual;

	case iblob::IBlobFilterParams::FC_BETWEEN:
		{
			bool valueInRange = filter.valueRange.Contains(value);
			return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? valueInRange: !valueInRange;
		}

	case iblob::IBlobFilterParams::FC_OUTSIDE:
		{
			bool valueInRange = filter.valueRange.Contains(value);
			return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? !valueInRange: valueInRange;
		}

	case iblob::IBlobFilterParams::FC_GREATER:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? isGreater: !isGreater;

	case iblob::IBlobFilterParams::FC_GREATER_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? isGreaterEqual: !isGreaterEqual;

	case iblob::IBlobFilterParams::FC_LESS:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? isLess: !isLess;

	case iblob::IBlobFilterParams::FC_LESS_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE)? isLessEqual: !isLessEqual;

	default:
		return true;
	}
}


// public methods of embedded class FilterFeatureList

CBlobProcessorCompBase::FilterFeatureList::FilterFeatureList()
{
	InsertOption("Area", areaFeatureId, "Blob area");
	InsertOption("Perimeter", perimeterFeatureId, "Total length of edges in a blob (including the edges of any holes)");
	InsertOption("Circularity", circularityFeatureId, "Circularity factor of the blob");
}


} // namespace iblob


