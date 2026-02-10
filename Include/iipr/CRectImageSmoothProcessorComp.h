// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CRectImageSmoothProcessorComp_included
#define iipr_CRectImageSmoothProcessorComp_included


// ACF-Solutions includes
#include <imeas/INumericValue.h>
#include <imeas/CGeneralNumericConstraints.h>
#include <iipr/CImageProcessorCompBase.h>


namespace iipr
{


/**
	Implementation of image smooth operator with homogeneous rectangle kernel.
	This implementation uses moving average alghorithm.
	\todo Implement using of AOI parameters from base class.
*/
class CRectImageSmoothProcessorComp: public CImageProcessorCompBase
{
public:
	typedef CImageProcessorCompBase BaseClass;

	enum UnitMode
	{
		UM_PIXEL,
		UM_PERCENT,
		UM_PERCENT_DIAG
	};

	enum BorderMode
	{
		BM_STRETCH_KERNEL,
		BM_REDUCE_OUTPUT
	};

	I_BEGIN_COMPONENT(CRectImageSmoothProcessorComp);
		I_REGISTER_SUBELEMENT(FilterContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(FilterContraints, imeas::INumericConstraints, ExtractFilterContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(FilterContraints, istd::IChangeable, ExtractFilterContraints);
		I_ASSIGN(m_unitModeAttrPtr, "UnitMode", "Define used units for filter kernel definition:\n\t0 - Pixels\n\t1 - Percent of width and height\n\t2 - Percent of diagonal", true, 0);
		I_ASSIGN(m_borderModeAttrPtr, "BorderMode", "Define used mode for border area:\n\t0 - Kernel will be stretched, original image size will be outputed\n\t1 - Border area removed, output image will be smaller", true, 0);
		I_ASSIGN(m_filterParamsIdAttrPtr, "FilterParamsId", "ID of filter length parameters in the parameter set (imeas::INumericValue)", false, "FilterParams");
		I_ASSIGN(m_defaultFilterParamsCompPtr, "DefaultFilterParams", "Default filter parameters", false, "DefaultFilterParams");
	I_END_COMPONENT;

	static bool DoRectFilter(
				int filterWidth,
				int filterHeight,
				iimg::IBitmap::PixelFormat outputPixelFormat,
				const iimg::IBitmap& inputBitmap,
				iimg::IBitmap& outputBitmap,
				BorderMode borderMode = BM_STRETCH_KERNEL);
	static bool DoRectFilterHorizontal(
				int filterWidth,
				iimg::IBitmap::PixelFormat outputPixelFormat,
				const iimg::IBitmap& inputBitmap,
				iimg::IBitmap& outputBitmap,
				BorderMode borderMode = BM_STRETCH_KERNEL);
	static bool DoRectFilterVertical(
				int filterHeight,
				iimg::IBitmap::PixelFormat outputPixelFormat,
				const iimg::IBitmap& inputBitmap,
				iimg::IBitmap& outputBitmap,
				BorderMode borderMode = BM_STRETCH_KERNEL);

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractFilterContraints(CRectImageSmoothProcessorComp& component)
	{
		return &component.m_filterConstraints;
	}

	I_ATTR(int, m_unitModeAttrPtr);
	I_ATTR(int, m_borderModeAttrPtr);
	I_ATTR(QByteArray, m_filterParamsIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultFilterParamsCompPtr);

	imeas::CGeneralNumericConstraints m_filterConstraints;
};


} // namespace iipr


#endif // !iipr_CRectImageSmoothProcessorComp_included


