#ifndef iipr_CMorphologicalProcessorComp_included
#define iipr_CMorphologicalProcessorComp_included


// ACF-Solutions includes
#include "iipr/CImageRegionProcessorCompBase.h"


namespace iipr
{


/**	
	Processor for image cropping.
*/
class CMorphologicalProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;

	enum ProcessingMode
	{
		PM_EROSION,
		PM_DILATATION,
		PM_OPENING,
		PM_CLOSING
	};
	
	I_BEGIN_COMPONENT(CMorphologicalProcessorComp);
		I_ASSIGN(m_filterSizeParamsIdAttrPtr, "FilterSizeParamsId", "ID of the filter dimension parameter set", true, "FilterSizeParamsId");
		I_ASSIGN(m_processingModeAttrPtr, "ProcessingMode", "Filter processing mode\n0 - Erosion\n1 - Dilatation\n2 - Opening\n3 - Closing", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;
private:
	I_ATTR(QByteArray, m_filterSizeParamsIdAttrPtr);
	I_ATTR(int, m_processingModeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CMorphologicalProcessorComp_included

