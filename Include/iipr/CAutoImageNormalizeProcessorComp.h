#ifndef iipr_CAutoImageNormalizeProcessorComp_included
#define iipr_CAutoImageNormalizeProcessorComp_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "i2d/CRectangle.h"

// ACF-Solutions includes
#include "imeas/INumericValue.h"

#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{


class CAutoImageNormalizeProcessorComp: public iipr::CImageProcessorCompBase
{
public:
	typedef iipr::CImageProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(CAutoImageNormalizeProcessorComp);
	I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of rectangle AOI area in parameter set, only this area will processed", false, "AoiParamId");
	I_ASSIGN(m_defaultAoiParamCompPtr, "DefaultAoiParam", "Default parameter for rectangle AOI area, if no parameters are specified", false, "DefaultAoiParam");
	I_ASSIGN(m_clippingThresholdIdAttrPtr, "ClippingThresholdId", "ID of the clipping threshold in parameter set, only this area will processed", false, "ClippingThresholdId");
	I_ASSIGN(m_defaultClippingThresholdCompPtr, "DefaultClippingThreshold", "Default parameter for clipping threshold, if no parameters are specified", false, "DefaultClippingThreshold");
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_REF(i2d::CRectangle, m_defaultAoiParamCompPtr);

	I_ATTR(QByteArray, m_clippingThresholdIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultClippingThresholdCompPtr);
};


} // namespace iipr


#endif // !iipr_CAutoImageNormalizeProcessorComp_included


