#pragma once


// ACF includes
#include <i2d/ICalibrationProvider.h>
#include <iimg/IBitmapProvider.h>
#include <iipr/IImageToFeatureProcessor.h>

// ACF-Solutions includes
#include <iinsp/TSupplierCompWrap.h>
#include <icalib/CPerspectiveCalibration2d.h>


namespace iipr
{


class CPerspectiveCalibrationSupplierComp:
			public iinsp::TSupplierCompWrap<icalib::CPerspectiveCalibration2d>,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap<icalib::CPerspectiveCalibration2d> BaseClass;

	I_BEGIN_COMPONENT(CPerspectiveCalibrationSupplierComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide input image", true, "BitmapProvider");
		I_ASSIGN(m_pointGridExtractorCompPtr, "PointGridExtractor", "Extractor of pointGrid on image", true, "PointGridExtractor");
		I_ASSIGN(m_pointGridTransformation, "PointGridTransformation", "Provides of 2D transformation of point grid in 2D coortinate space", false, "PointGridTransformation");
		I_ASSIGN(m_cellSizeParamId, "CellSizeParamsId", "ID of point grid cell size in parameter set (type imeas::INumericValue): cell size in logical units", false, "CellSizeParam");
		I_ASSIGN(m_defaultCellSizeAttrPtr, "DefaultCellSize", "Default size of single cell (in logical units) if no checkboard parameters specified", true, 10);
		I_ASSIGN(m_invertXAttrPtr, "InvertX", "Invert x direction", false, false);
		I_ASSIGN(m_invertYAttrPtr, "InvertY", "Invert y direction", false, false);
	I_END_COMPONENT;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	bool CalculateCalibration(const iimg::IBitmap& image, icalib::CPerspectiveCalibration2d& result) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const;

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iipr::IImageToFeatureProcessor, m_pointGridExtractorCompPtr);
	I_REF(i2d::ICalibrationProvider, m_pointGridTransformation);
	I_ATTR(QByteArray, m_cellSizeParamId);
	I_ATTR(double, m_defaultCellSizeAttrPtr);
	I_ATTR(bool, m_invertXAttrPtr);
	I_ATTR(bool, m_invertYAttrPtr);
};


} // namespace iipr


