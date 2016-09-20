#pragma once


// ACF includes
#include <iimg/IBitmap.h>
#include <icam/CCameraDelegatorComp.h>


namespace icam
{


/**
	Camera provided bitmaps with integrated calibration information.
*/
class CCalibratedCameraComp: public icam::CCameraDelegatorComp
{
public:
	enum{
		RESOLUTION_PATTERN = 0xff00ff00
	};

	typedef icam::CCameraDelegatorComp BaseClass;

	I_BEGIN_COMPONENT(CCalibratedCameraComp);
		I_ASSIGN(m_scaleParamIdAttrPtr, "ScaleParamId", "ID of the scale parameter in the parameter set", true, "Scale");
	I_END_COMPONENT;

	static bool ReadImageResolution(const iimg::IBitmap& bitmap, double& resolution);
	static bool WriteImageResolution(iimg::IBitmap& bitmap, double resolution);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

private:
	I_ATTR(QByteArray, m_scaleParamIdAttrPtr);
};


} // namespace icam


