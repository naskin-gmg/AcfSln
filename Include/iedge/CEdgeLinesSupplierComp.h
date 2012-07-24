#ifndef iedge_CEdgeLinesSupplierComp_included
#define iedge_CEdgeLinesSupplierComp_included


// ACF includes
#include "iproc/TSupplierCompWrap.h"
#include "i2d/ICalibrationProvider.h"

// ACF-Solutions includes
#include "iipr/IBitmapProvider.h"

#include "iedge/IEdgeLinesProvider.h"
#include "iedge/IEdgesExtractor.h"
#include "iedge/CEdgeLine.h"


namespace iedge
{


class CEdgeLinesSupplierComp:
			public iproc::TSupplierCompWrap<CEdgeLine::Container>,
			virtual public IEdgeLinesProvider
{
public:
	typedef iproc::TSupplierCompWrap<CEdgeLine::Container> BaseClass;

	I_BEGIN_COMPONENT(CEdgeLinesSupplierComp);
		I_REGISTER_INTERFACE(IEdgeLinesProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide image to analyse", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationSupplier", "Provide 2D-calibration object", false, "CalibrationSupplier");
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, false);
		I_ASSIGN(edgesExtractorCompPtr, "EdgesExtractor", "Extractor of edges", true, "EdgesExtractor");
	I_END_COMPONENT;

	// reimplemented (iedge::IEdgeLinesProvider)
	virtual const CEdgeLine::Container* GetEdgesContainer() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(CEdgeLine::Container& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iipr::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);
	I_REF(IEdgesExtractor, edgesExtractorCompPtr);
};


} // namespace iedge


#endif // !iedge_CEdgeLinesSupplierComp_included


