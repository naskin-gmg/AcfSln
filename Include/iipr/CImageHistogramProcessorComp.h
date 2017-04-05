#ifndef iipr_CImageHistogramProcessorComp_included
#define iipr_CImageHistogramProcessorComp_included


// Qt includes
#include <QtCore/QStringList>

// ACF-Solutions includes
#include <imeas/IDiscreteDataSequence.h>
#include <iipr/CImageRegionProcessorCompBase.h>


namespace iipr
{


/**	
	Implementation of a processor for the image histogram calculation.
*/
class CImageHistogramProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImageHistogramProcessorComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;
};


} // namespace iipr


#endif // !iipr_CImageHistogramProcessorComp_included

