#ifndef iipr_CLensCorrFindSupplierComp_included
#define iipr_CLensCorrFindSupplierComp_included


// ACF includes
#include <imath/CFixedPointManip.h>
#include <imath/CGeneralUnitInfo.h>
#include <i2d/ICalibrationProvider.h>
#include <iimg/IBitmapProvider.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>
#include <imeas/CGeneralNumericConstraints.h>
#include <iproc/IProcessor.h>
#include <iinsp/TSupplierCompWrap.h>
#include <icalib/CSimpleLensCorrection.h>
#include <iipr/IImageToFeatureProcessor.h>
#include <iipr/CHoughSpace2d.h>


namespace iipr
{


/**
	Supplier of bitmap built on difference of two images.
*/
class CLensCorrFindSupplierComp:
			public iinsp::TSupplierCompWrap< icalib::CSimpleLensCorrection >,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap<icalib::CSimpleLensCorrection> BaseClass;

	I_BEGIN_COMPONENT(CLensCorrFindSupplierComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide input image", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_pointFinderCompPtr, "PointFinder", "Finder of points on image", true, "PointFinder");
		I_ASSIGN(m_defaultAngleResAttrPtr, "DefaultAngleResulution", "Resolution of angle grid", true, 360);
		I_ASSIGN(m_defaultRadiusResAttrPtr, "DefaultRadiusResulution", "Resolution of radius grid", true, 200);
		I_ASSIGN(m_defaultMaxLinesAttrPtr, "DefaultMaxLines", "Number of maximal found lines", true, 100);
		I_ASSIGN(m_defaultSmoothKernelAttrPtr, "DefaultSpaceSmoothKernel", "Size of kernel will be used to smooth hough space", true, 10);
		I_ASSIGN(m_defaultMinDistanceAttrPtr, "DefaultMinDistance", "Default value of minimal line distance to center", true, 100);
	I_END_COMPONENT;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	struct FeatureInfo
	{
		double weight;
		i2d::CVector2d position;
	};

	typedef QList<FeatureInfo> Features;

	class FeaturesConsumer: virtual public iipr::IFeaturesConsumer
	{
	public:
		// reimplemented (iipr::IFeaturesConsumer)
		virtual void ResetFeatures();
		virtual bool AddFeature(const imeas::INumericValue* featurePtr, bool* isFullPtr = NULL);
	
		Features features;
	};

	struct CorrectionInfo
	{
		i2d::CVector2d position;
		i2d::CVector2d diff;
	};
	typedef QList<CorrectionInfo> CorrectionInfos;

	struct CorrectionLineInfo
	{
		CorrectionInfos infos;
		i2d::CVector2d orthoVector;
	};

	bool CalculateCalibration(const iimg::IBitmap& image, icalib::CSimpleLensCorrection& result) const;
	i2d::CVector2d CalcHoughPos(const i2d::CVector2d& point1, const i2d::CVector2d& point2, const i2d::CVector2d& imageCenter, const istd::CIndex2d& spaceSize) const;
	i2d::CLine2d CalcCorrespondingLine(const i2d::CVector2d& houghPos, const i2d::CVector2d& imageCenter, const istd::CIndex2d& spaceSize) const;
	void UpdateHoughSpace(const i2d::CVector2d& point1, const i2d::CVector2d& point2, double weight, const i2d::CVector2d& imageCenter, CHoughSpace2d& space) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapProviderSupplierCompPtr);
	I_REF(iipr::IImageToFeatureProcessor, m_pointFinderCompPtr);
	I_ATTR(int, m_defaultAngleResAttrPtr);
	I_ATTR(int, m_defaultRadiusResAttrPtr);
	I_ATTR(int, m_defaultMaxLinesAttrPtr);
	I_ATTR(int, m_defaultSmoothKernelAttrPtr);
	I_ATTR(int, m_defaultMinDistanceAttrPtr);
};


} // namespace iipr


#endif // !iipr_CLensCorrFindSupplierComp_included


