#ifndef iinsp_CCheckboardCalibSupplierComp_included
#define iinsp_CCheckboardCalibSupplierComp_included


// ACF includes
#include "i2d/ICalibrationProvider.h"
#include "i2d/CPerspectiveCalibration2d.h"
#include "iimg/IBitmapProvider.h"

// ACF-Solutions includes
#include "iproc/IProcessor.h"
#include "iipr/IImageToFeatureProcessor.h"
#include "iinsp/TSupplierCompWrap.h"


namespace iinsp
{


/**
	Supplier of bitmap built on difference of two images.
*/
class CCheckboardCalibSupplierComp:
			public iinsp::TSupplierCompWrap< i2d::CPerspectiveCalibration2d >,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap<i2d::CPerspectiveCalibration2d> BaseClass;

	I_BEGIN_COMPONENT(CCheckboardCalibSupplierComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide input image", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_lineFinderCompPtr, "LineFinder", "Finder of lines on image", true, "LineFinder");
	I_END_COMPONENT;

protected:
	class LinesConsumer: virtual public iipr::IFeaturesConsumer
	{
	public:
		// reimplemented (iipr::IFeaturesConsumer)
		virtual void ResetFeatures();
		virtual bool AddFeature(const imeas::INumericValue* featurePtr, bool* isFullPtr = NULL);
	
		struct LineInfo
		{
			double weight;
			i2d::CLine2d line;
		};

		typedef QList<LineInfo> Lines;
		Lines lines;
	};

	bool CalculateCalibration(const iimg::IBitmap& image, i2d::CPerspectiveCalibration2d& result) const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapProviderSupplierCompPtr);
	I_REF(iipr::IImageToFeatureProcessor, m_lineFinderCompPtr);
};


} // namespace iinsp


#endif // !iinsp_CCheckboardCalibSupplierComp_included


