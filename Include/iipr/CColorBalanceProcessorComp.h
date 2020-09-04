#pragma once

// ACF includes
#include <imeas/INumericValue.h>

// ACF-Solutions includes
#include <iipr/CImageProcessorCompBase.h>


namespace iipr
{

/**
	Implementation of image processor applying color balancing coefficients to input color image
*/

class CColorBalanceProcessorComp: public iipr::CImageProcessorCompBase
{
public:
	typedef iipr::CImageProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(CColorBalanceProcessorComp);
		I_ASSIGN(m_colorCoefficientsIdAttrPtr, "ColorCoefficientsId", "Id of color coefficients (imeas::INumericParam) in ParamsSet", true, "ColorCoefficientsId");
		I_ASSIGN(m_defaultColorCoefsCompPtr, "DefaultColorCoefficients", "Default color coefficients", true, "DefaultColorCoefficients");
	I_END_COMPONENT;


protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) const;

private:
	void AssignWeights(imath::CVarVector& params, int& red, int& green, int& blue) const;
	double GetReductionFactor(double red, double green, double blue) const;
	bool ValidateColorCoefficients(const imath::CVarVector& colorCoefficients) const;

private:
	I_ATTR(QByteArray, m_colorCoefficientsIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultColorCoefsCompPtr);
};


} // namespace iipr

