#ifndef iinsp_CCheckboardCalibSupplierComp_included
#define iinsp_CCheckboardCalibSupplierComp_included


// ACF includes
#include "imath/CFixedPointManip.h"
#include "imath/CGeneralUnitInfo.h"
#include "i2d/ICalibrationProvider.h"
#include "i2d/CPerspectiveCalibration2d.h"
#include "iimg/IBitmapProvider.h"

// ACF-Solutions includes
#include "imeas/INumericValue.h"
#include "imeas/CGeneralNumericConstraints.h"
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
		I_REGISTER_SUBELEMENT(CheckboardParamContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(CheckboardParamContraints, imeas::INumericConstraints, ExtractCheckboardParamContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(CheckboardParamContraints, istd::IChangeable, ExtractCheckboardParamContraints);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide input image", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_lineFinderCompPtr, "LineFinder", "Finder of lines on image", true, "LineFinder");
		I_ASSIGN(m_checkboardParamsId, "CheckboardParamsId", "ID of checkboard parameters in parameter set (type imeas::INumericValue):\n1\t1. grid size\n\t2. cell size in logical units", false, "CheckboardParams");
		I_ASSIGN(m_defaultCheckboardParamsCompPtr, "DefaultCheckboardParams", "Default parameter controlling size of single checkboard cell when it is not defined in parameter set:\n1\t1. grid size\n\t2. cell size in logical units", false, "DefaultCheckboardParams");
		I_ASSIGN(m_distanceUnitInfoCompPtr, "DistanceUnitInfo", "Information about used distance units", false, "DistanceUnitInfo");
		I_ASSIGN(m_defaultGridSizeAttrPtr, "DefaultGridSize", "Default size of grid if no checkboard parameters specified", true, 8);
		I_ASSIGN(m_defaultCellSizeAttrPtr, "DefaultCellSize", "Default size of single cell (in logical units) if no checkboard parameters specified", true, 10);
	I_END_COMPONENT;

	CCheckboardCalibSupplierComp();

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	struct LineInfo
	{
		double weight;
		i2d::CLine2d line;
	};

	typedef QList<LineInfo> Lines;

	class LinesConsumer: virtual public iipr::IFeaturesConsumer
	{
	public:
		// reimplemented (iipr::IFeaturesConsumer)
		virtual void ResetFeatures();
		virtual bool AddFeature(const imeas::INumericValue* featurePtr, bool* isFullPtr = NULL);
	
		Lines lines;
	};

	class ChessboardParamsContraints: public imeas::INumericConstraints
	{
	public:
		ChessboardParamsContraints(const CCheckboardCalibSupplierComp* supplierPtr);

		// reimplemented (imeas::INumericConstraints)
		virtual int GetNumericValuesCount() const;
		virtual QString GetNumericValueName(int index) const;
		virtual QString GetNumericValueDescription(int index) const;
		virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const;

	private:
		imath::CGeneralUnitInfo m_cellSizeUnit;
		const CCheckboardCalibSupplierComp* m_parentPtr;
	};

	bool CalculateCalibration(const iimg::IBitmap& image, i2d::CPerspectiveCalibration2d& result) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapProviderSupplierCompPtr);
	I_REF(iipr::IImageToFeatureProcessor, m_lineFinderCompPtr);
	I_ATTR(QByteArray, m_checkboardParamsId);
	I_REF(imeas::INumericValue, m_defaultCheckboardParamsCompPtr);
	I_REF(imath::IUnitInfo, m_distanceUnitInfoCompPtr);
	I_ATTR(int, m_defaultGridSizeAttrPtr);
	I_ATTR(double, m_defaultCellSizeAttrPtr);

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractCheckboardParamContraints(CCheckboardCalibSupplierComp& component)
	{
		return &component.m_checkboardParamContraints;
	}

	ChessboardParamsContraints m_checkboardParamContraints;

	static imath::CFixedPointManip s_gridSizeValueManip;
	static imath::CGeneralUnitInfo s_gridSizeUnit;
};


} // namespace iinsp


#endif // !iinsp_CCheckboardCalibSupplierComp_included


