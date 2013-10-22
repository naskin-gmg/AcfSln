#include "iipr/CColorPatternComparatorComp.h"


// ACF includes
#include "i2d/CDirection2d.h"
#include "iprm/TParamsPtr.h"
#include "icmm/CVarColor.h"
#include "icmm/CRgbToHsvTranformation.h"
#include "icmm/CRgb.h"

// ACF-Solutions includes
#include "imeas/CDataSequenceStatistics.h"

#include "iipr/IPatternController.h"


namespace iipr
{


// public methods

CColorPatternComparatorComp::CColorPatternComparatorComp()
	:m_isColorPatternMatched(false)
{
}

// reimplemented (imeas::INumericValueProvider)

int CColorPatternComparatorComp::GetValuesCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return 1;
	}

	return 0;
}


const imeas::INumericValue& CColorPatternComparatorComp::GetNumericValue(int I_IF_DEBUG(index)) const
{
	Q_ASSERT(index == 0);

	static imeas::CSimpleNumericValue emptyValue;

	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return *productPtr;
	}

	return emptyValue;
}


// reimplemented (istd::IInformationProvider)

QDateTime CColorPatternComparatorComp::GetInformationTimeStamp() const
{
	return m_resultTimeStamp;
}


istd::IInformationProvider::InformationCategory CColorPatternComparatorComp::GetInformationCategory() const
{
	int workStatus = GetWorkStatus();

	switch (workStatus){
		case WS_OK:
			if (!m_isColorPatternMatched){
				return IC_ERROR;
			}
			return IC_INFO;

		case WS_ERROR:
			return IC_ERROR;

		case WS_CRITICAL:
			return IC_CRITICAL;

		default:
			return IC_NONE;
	}

	return IC_NONE;
}


int CColorPatternComparatorComp::GetInformationId() const
{
	return -1;
}


QString CColorPatternComparatorComp::GetInformationDescription() const
{
	if (m_isColorPatternMatched){
		return QObject::tr("Color Check OK");
	}
	
	return QObject::tr("Color Check NOT OK");
}


QString CColorPatternComparatorComp::GetInformationSource() const
{
	return *m_defaultSourceAttrPtr;
}


int CColorPatternComparatorComp::GetInformationFlags() const
{
	return ITF_SYSTEM;
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CColorPatternComparatorComp::ProduceObject(ProductType& result) const
{
	result.SetValues(imath::CVarVector());

	m_resultTimeStamp = QDateTime::currentDateTime();

	if (!m_workingPatternProviderCompPtr.IsValid() || !m_teachedPatternProviderCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'WorkingPatternProvider' or 'TeachedPatternProvider' component references are not set");

		return WS_CRITICAL;
	}

	if (!m_dataStatisticsProcessorCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'DataStatisticsProcessor' component references are not set");

		return WS_CRITICAL;
	}

	const imeas::IDataSequence* workingHistogramPtr = m_workingPatternProviderCompPtr->GetDataSequence();
	if (workingHistogramPtr == NULL){
		SendErrorMessage(0, "Working histogram not available");

		return WS_ERROR;
	}

	const imeas::IDataSequence* teachedHistogramPtr = m_teachedPatternProviderCompPtr->GetDataSequence();
	if (teachedHistogramPtr == NULL){
		SendErrorMessage(0, "Teached histogram not available");

		return WS_ERROR;
	}

	int workingSamplesCount = workingHistogramPtr->GetSamplesCount();
	int teachedSamplesCount= teachedHistogramPtr->GetSamplesCount();
	if (workingSamplesCount != teachedSamplesCount){
		SendErrorMessage(0, "Working and teached histograms have different dimensions");

		return WS_ERROR;
	}

	int channelsCount = teachedHistogramPtr->GetChannelsCount();

	if ((workingHistogramPtr->GetChannelsCount() != channelsCount) || ((channelsCount != 3) && (channelsCount != 1))){
		SendErrorMessage(0, "A histogram has wrong dimension. Only histograms for gray value and RGB images are supported");

		return WS_ERROR;	
	}

	imeas::CDataSequenceStatistics workingStatistics;
	if (m_dataStatisticsProcessorCompPtr->CalculateDataStatistics(*workingHistogramPtr, workingStatistics) != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Calculation of the histogram statistics data failed");

		return WS_ERROR;
	}

	imeas::CDataSequenceStatistics teachedStatistics;
	if (m_dataStatisticsProcessorCompPtr->CalculateDataStatistics(*teachedHistogramPtr, teachedStatistics) != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Calculation of the histogram statistics data failed");

		return WS_ERROR;
	}

	iprm::TParamsPtr<imeas::INumericValue> comparsionThresholdPtr(GetModelParametersSet(), *m_patternCompareThresholdParamIdAttrPtr);
	if (!comparsionThresholdPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, comparison threshold attribute is invalid");

		return WS_CRITICAL;
	}

	Q_ASSERT(teachedStatistics.GetChannelsCount() == workingStatistics.GetChannelsCount());

	imath::CVarVector thresholdValues = comparsionThresholdPtr->GetValues();
	if (thresholdValues.GetElementsCount() != channelsCount){
		SendErrorMessage(0, "Dimension of the threshold parameter is not corresponding with the given histogram dimension");

		return WS_ERROR;
	}

	// Input image is RGB. Differences between teached and working images are done in HSV color space:
	if (channelsCount == 3){
		
		icmm::CHsv teachedHsvColor;
		if (!GetHsvColorValue(teachedStatistics, teachedHsvColor)){
			return WS_ERROR;
		}

		icmm::CHsv workingHsvColor;
		if (!GetHsvColorValue(workingStatistics, workingHsvColor)){
			return WS_ERROR;
		}

		imath::CVarVector values(6);
		values[0] = workingHsvColor.GetHue();
		values[1] = workingHsvColor.GetSaturation() * 255;
		values[2] = workingHsvColor.GetValue() * 255;

		values[3] = teachedHsvColor.GetHue();
		values[4] = teachedHsvColor.GetSaturation() * 255;
		values[5] = teachedHsvColor.GetValue() * 255;

		result.SetValues(values);

		i2d::CDirection2d teachedHue = i2d::CDirection2d::FromDegree(teachedHsvColor.GetHue());
		i2d::CDirection2d workingHue = i2d::CDirection2d::FromDegree(workingHsvColor.GetHue());

		double hueDifference = workingHue.DistInDegree(teachedHue) / 360.0;
		double saturationDifference = fabs(teachedHsvColor.GetSaturation() - workingHsvColor.GetSaturation());
		double valueDifference = fabs(teachedHsvColor.GetValue() - workingHsvColor.GetValue());

		double maxHueDifference = thresholdValues[0];
		double maxSaturationDifference = thresholdValues[1];
		double maxValueDifference = thresholdValues[2];

		m_isColorPatternMatched =
					(hueDifference <= maxHueDifference) && 
					(saturationDifference <= maxSaturationDifference) && 
					(valueDifference <= maxValueDifference);
	}

	else if(channelsCount == 1){
		const imeas::IDataStatistics* teachedStatisticsPtr = teachedStatistics.GetChannelStatistics(0);
		Q_ASSERT(teachedStatisticsPtr != NULL);
		double teachedMedian = teachedStatisticsPtr->GetMedian();

		const imeas::IDataStatistics* workingStatisticsPtr = workingStatistics.GetChannelStatistics(0);
		Q_ASSERT(teachedStatisticsPtr != NULL);
		double workingMedian = workingStatisticsPtr->GetMedian();

		double medianDifference = fabs(workingMedian - teachedMedian);

		m_isColorPatternMatched = (medianDifference <= thresholdValues[0]);

		imath::CVarVector values(2);
		values[0] = workingMedian;
		values[1] = teachedMedian;

		result.SetValues(values);
	}
	else{
		I_CRITICAL();
	}

	return WS_OK;
}


// reimplemented (icomp::CComponentBase)

void CColorPatternComparatorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_workingPatternProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_workingPatternProviderModelCompPtr.GetPtr(), m_workingPatternSupplierCompPtr.GetPtr());
	}

	if (m_teachedPatternProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_teachedPatternProviderModelCompPtr.GetPtr(), m_teachedPatternSupplierCompPtr.GetPtr());
	}

	// Force components initialization
	m_workingPatternProviderCompPtr.EnsureInitialized();
	m_teachedPatternProviderCompPtr.EnsureInitialized();
	m_dataStatisticsProcessorCompPtr.EnsureInitialized();
}


void CColorPatternComparatorComp::OnComponentDestroyed()
{
	if (m_workingPatternProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_workingPatternProviderModelCompPtr.GetPtr());
	}

	if (m_teachedPatternProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_teachedPatternProviderModelCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


// private methods

bool CColorPatternComparatorComp::GetHsvColorValue(const imeas::IDataSequenceStatistics& statistics, icmm::CHsv& hsv) const
{
	Q_ASSERT(statistics.GetChannelsCount() == 3);

	const imeas::IDataStatistics* redStatisticsPtr = statistics.GetChannelStatistics(0);
	const imeas::IDataStatistics* greenStatisticsPtr = statistics.GetChannelStatistics(1);
	const imeas::IDataStatistics* blueStatisticsPtr = statistics.GetChannelStatistics(2);

	Q_ASSERT(redStatisticsPtr != NULL);
	Q_ASSERT(greenStatisticsPtr != NULL);
	Q_ASSERT(blueStatisticsPtr != NULL);

	double redMedian = redStatisticsPtr->GetAverage();
	double greenMedian = greenStatisticsPtr->GetAverage(); 
	double blueMedian = blueStatisticsPtr->GetAverage();

	icmm::CRgbToHsvTranformation rgbToHsv;
	icmm::CRgb rgbMedian(redMedian, greenMedian, blueMedian);
	icmm::CVarColor rgbColor(rgbMedian);
	icmm::CVarColor hsvColor(3);

	if (!rgbToHsv.GetValueAt(rgbColor, hsvColor)){
		return false;
	}

	hsv = icmm::CHsv(hsvColor);

	return true;
}



} // namespace iipr


