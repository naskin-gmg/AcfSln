#include "iinsp/CCheckboardCalibSupplierComp.h"


namespace iinsp
{


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CCheckboardCalibSupplierComp::GetCalibration() const
{
	return GetWorkProduct();
}


//protected methods

bool CCheckboardCalibSupplierComp::CalculateCalibration(const iimg::IBitmap& image, i2d::CPerspectiveCalibration2d& result) const
{
	result.Reset();

	if (!m_lineFinderCompPtr.IsValid()){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_CRITICAL, 0, QObject::tr("No processor set"), "CheckboardCalibSupplier"));

		return false;
	}

	LinesConsumer processedLines;
	if (m_lineFinderCompPtr->DoExtractFeatures(GetModelParametersSet(), image, processedLines) != iproc::IProcessor::TS_OK){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("Cannot extract lines from image"), "CheckboardCalibSupplier"));

		return false;
	}

	if (processedLines.lines.size() >= 28){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("No enough lines found"), "CheckboardCalibSupplier"));

		return false;
	}

	result.Reset();

	return true;
}


// reimplemented (iinsp::TSupplierCompWrap)

int CCheckboardCalibSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapProviderCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("Input image could not be provided"), "CheckboardCalibSupplier"));

		return WS_ERROR;
	}

	Timer performanceTimer(this, "Checkboard calibration");

	if (!CalculateCalibration(*bitmapPtr, result)){
		return WS_ERROR;
	}

	return WS_OK;
}


// reimplemented (icomp::CComponentBase)

void CCheckboardCalibSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapProviderSupplierCompPtr.GetPtr());
	}
}


// public methods of embedded class LinesConsumer

// reimplemented (iipr::IFeaturesConsumer)

void CCheckboardCalibSupplierComp::LinesConsumer::ResetFeatures()
{
	lines.clear();
}


bool CCheckboardCalibSupplierComp::LinesConsumer::AddFeature(const imeas::INumericValue* featurePtr, bool* /*isFullPtr*/)
{
	if (featurePtr != NULL){
		imath::CVarVector vector = featurePtr->GetValues();
		if (vector.GetElementsCount() >= 4){
			LineInfo info;

			info.line = i2d::CLine2d(vector[0], vector[1], vector[2], vector[3]);
			if (vector.GetElementsCount() >= 5){
				info.weight = vector[5];
			}
			else{
				info.weight = 1;
			}
		}
	}

	return true;
}



} // namespace iinsp


