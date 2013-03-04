#ifndef iedge_CExtractedEdgeLinesSupplierComp_included
#define iedge_CExtractedEdgeLinesSupplierComp_included


// ACF includes
#include "i2d/ICalibrationProvider.h"

// ACF-Solutions includes
#include "iproc/ISupplier.h"
#include "iproc/TSupplierCompWrap.h"
#include "iipr/IBitmapProvider.h"

#include "iedge/IEdgeLinesProvider.h"
#include "iedge/IEdgesExtractor.h"
#include "iedge/CEdgeLineContainer.h"


namespace iedge
{


class CExtractedEdgeLinesSupplierComp:
			public iproc::TSupplierCompWrap<CEdgeLineContainer>,
			virtual public IEdgeLinesProvider
{
public:
	typedef iproc::TSupplierCompWrap<CEdgeLineContainer> BaseClass;

	I_BEGIN_COMPONENT(CExtractedEdgeLinesSupplierComp);
		I_REGISTER_INTERFACE(IEdgeLinesProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provide 2D-calibration object", false, "CalibrationProvider");
		I_ASSIGN_TO(m_calibrationSupplierCompPtr, m_calibrationProviderCompPtr, false);
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, false);
		I_ASSIGN(m_edgesExtractorCompPtr, "EdgesExtractor", "Extractor of edges", true, "EdgesExtractor");
	I_END_COMPONENT;

	// reimplemented (iedge::IEdgeLinesProvider)
	virtual const CEdgeLineContainer* GetEdgesContainer() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(CEdgeLineContainer& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iipr::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iproc::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(iproc::ISupplier, m_calibrationSupplierCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);
	I_REF(IEdgesExtractor, m_edgesExtractorCompPtr);
};


} // namespace iedge


#endif // !iedge_CExtractedEdgeLinesSupplierComp_included


