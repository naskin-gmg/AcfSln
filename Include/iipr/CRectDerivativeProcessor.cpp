#include "iipr/CRectDerivativeProcessor.h"


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "imath/CDoubleManip.h"
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/CGeneralDataSequence.h"
#include "imeas/INumericValue.h"
#include "imeas/CSamplesInfo.h"


namespace iipr
{


// public

CRectDerivativeProcessor::CRectDerivativeProcessor()
:	m_doublePrecision(false)
{
}




bool CRectDerivativeProcessor::DoDerivativeProcessing(const imeas::IDataSequence& source, double filterLength, imeas::IDataSequence& results, bool doublePrecision)
{
	int samplesCount = source.GetSamplesCount();
	int channelsCount = source.GetChannelsCount();
	if ((samplesCount < 2) || (channelsCount < 1)){
		return false;
	}

	istd::CChangeNotifier notifier(&results);

	istd::CRange sourceProportionRange(0, 1);

	const imeas::CSamplesInfo* sourceInfoPtr = dynamic_cast<const imeas::CSamplesInfo*>(source.GetSequenceInfo());
	if (sourceInfoPtr != NULL){
		const istd::CRange& logicalRange = sourceInfoPtr->GetLogicalSamplesRange();
		if (logicalRange.IsValid()){
			sourceProportionRange = logicalRange;
		}
	}

	int projectionWidth = samplesCount - 1;

	double proportionXAlpha = 0.5 / samplesCount;
	istd::CRange resultProportionRange(
				sourceProportionRange.GetValueFromAlpha(proportionXAlpha),
				sourceProportionRange.GetValueFromAlpha(1.0 - proportionXAlpha));
	if (!results.CreateSequenceWithInfo(
				istd::TSmartPtr<const imeas::IDataSequenceInfo>(new imeas::CSamplesInfo(resultProportionRange)),
				projectionWidth,
				channelsCount)){
		return false;
	}

	if (projectionWidth > 0){
		// special case for imeas::CGeneralDataSequence gives up to 25% performance gain
		const imeas::CGeneralDataSequence* inputSequencePtr = dynamic_cast<const imeas::CGeneralDataSequence*>(&source);
		imeas::CGeneralDataSequence* resultsSequencePtr = dynamic_cast<imeas::CGeneralDataSequence*>(&results);

		if ((channelsCount == 1) && (inputSequencePtr != NULL) && (resultsSequencePtr != NULL)){
			const double* channelData = inputSequencePtr->GetSamplesBuffer();
			double* resultsData = resultsSequencePtr->GetSamplesBuffer();

			if (doublePrecision){
				return CalculateDerivative<double, double, double>(channelData, samplesCount, filterLength, resultsData);
			}
			else{
				return CalculateDerivative<double, double, float>(channelData, samplesCount, filterLength, resultsData);
			}
		}

		// Default samples processing
		if (doublePrecision){
			return CalculateDerivative<double>(source, filterLength, results);
		}
		else{
			return CalculateDerivative<float>(source, filterLength, results);
		}
	}

	return true;
}


// reimplemented (iproc::IProcessor)

int CRectDerivativeProcessor::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	const imeas::IDataSequence* inputProjectionPtr = dynamic_cast<const imeas::IDataSequence*>(inputPtr);
	imeas::IDataSequence* outputProjectionPtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);

	double filterLength = 5.0;
	if (paramsPtr != NULL){
		iprm::TParamsPtr<imeas::INumericValue> filterParamsPtr(paramsPtr, m_filterParamsId);
		if (filterParamsPtr.IsValid()){
			imath::CVarVector filterLengths = filterParamsPtr->GetValues();
			if (filterLengths.GetElementsCount() > 0){
				filterLength = filterLengths[0];
			}
		}
	}

	return DoDerivativeProcessing(*inputProjectionPtr, filterLength, *outputProjectionPtr, m_doublePrecision)?
				TS_OK:
				TS_INVALID;
}


// reimplemented (imeas::INumericConstraints)

int CRectDerivativeProcessor::GetNumericValuesCount() const
{
	return 1;
}


QString CRectDerivativeProcessor::GetNumericValueName(int /*dimension*/) const
{
	return QObject::tr("Filter");
}


QString CRectDerivativeProcessor::GetNumericValueDescription(int /*index*/) const
{
	return QObject::tr("Smoothing filter length, higher value makes insensible to data noise");
}


const imath::IUnitInfo& CRectDerivativeProcessor::GetNumericValueUnitInfo(int /*dimension*/) const
{
	return *this;
}


// reimplemented (imath::IUnitInfo)

int CRectDerivativeProcessor::GetUnitType() const
{
	return UT_TECHNICAL;
}


QString CRectDerivativeProcessor::GetUnitName() const
{
	return "px";
}


double CRectDerivativeProcessor::GetDisplayMultiplicationFactor() const
{
	return 1;
}


istd::CRange CRectDerivativeProcessor::GetValueRange() const
{
	return istd::CRange(1, 100);
}


const imath::IDoubleManip& CRectDerivativeProcessor::GetValueManip() const
{
	static imath::CDoubleManip manip;

	return manip;
}


} // namespace iipr


