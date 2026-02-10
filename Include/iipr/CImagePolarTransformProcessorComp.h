// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CImagePolarTransformProcessorComp_included
#define iipr_CImagePolarTransformProcessorComp_included


// ACF-Solutions includes
#include <iipr/CImageRegionProcessorCompBase.h>


namespace iipr
{


/**	
	Implementation of a processor for the polar tranformation of an image region.
*/
class CImagePolarTransformProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImagePolarTransformProcessorComp);
		I_ASSIGN(m_angleResolutionParamIdAttrPtr, "AngleResolutionParamId", "ID of the angle resolution parameter in the parameter set", true, "AngleResolution");
		I_ASSIGN(m_interpolationParamIdAttrPtr, "InterpolationParamId", "ID of the interpolation parameter in the parameter set", true, "Interpolation");
		I_ASSIGN(m_extraUnrollingAttrPtr, "ExtraUnrollingParamId", "ID of the extra unrolling value parameter in the parameter set", true, "ExtraUnrolling");
	I_END_COMPONENT;

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;

private:
	I_ATTR(QByteArray, m_angleResolutionParamIdAttrPtr);
	I_ATTR(QByteArray, m_interpolationParamIdAttrPtr);
	I_ATTR(QByteArray, m_extraUnrollingAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImagePolarTransformProcessorComp_included

