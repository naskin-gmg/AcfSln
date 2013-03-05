#ifndef iipr_CColorPatternControllerComp_included
#define iipr_CColorPatternControllerComp_included


// ACF includes
#include "iser/ISerializable.h"
#include "imod/TModelWrap.h"
#include "ibase/TLoggerCompWrap.h"
#include "iimg/CGeneralBitmap.h"

// ACF-Solutions includes
#include "iproc/IProcessor.h"

#include "imeas/IDataSequenceProvider.h"
#include "imeas/TDiscreteDataSequence.h"

#include "iipr/IPatternController.h"
#include "iipr/IBitmapProvider.h"


namespace iipr
{


class CColorPatternControllerComp:
			virtual public ibase::CLoggerComponentBase,
			virtual public IPatternController,
			virtual public iipr::IBitmapProvider,
			virtual public imeas::IDataSequenceProvider,
			virtual public iser::ISerializable

{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CColorPatternControllerComp);
		I_REGISTER_INTERFACE(IPatternController);
		I_REGISTER_INTERFACE(iipr::IBitmapProvider);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_patternBitmapProviderCompPtr, "PatternBitmapProvider", "Provider of the bitmap pattern data used for teaching", true, "PatternBitmapProvider");
		I_ASSIGN(m_bitmapRegionCompPtr, "TeachingRegion", "Bitmap region used for teaching", true, "TeachingRegion");
		I_ASSIGN(m_histogramProgressorCompPtr, "HistogramProcessor", "Processor used for histogram calculation", true, "HistogramProcessor");
	I_END_COMPONENT;

	// reimplemented (IPatternController)
	virtual bool TeachPattern();
	virtual void ResetPattern();
	virtual bool IsPatternValid() const;
	const iser::ISerializable* GetPatternObject() const;

	// reimplemented (iipr::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (imeas::IDataSequenceProvider)
	virtual const imeas::IDataSequence* GetDataSequence() const;
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive & archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	imod::TModelWrap<iimg::CGeneralBitmap> m_patternBitmap;
	imod::TModelWrap<imeas::CSimpleSamplesSequence32> m_histogram;

	I_REF(iipr::IBitmapProvider, m_patternBitmapProviderCompPtr);
	I_REF(i2d::IObject2d, m_bitmapRegionCompPtr);
	I_REF(iproc::IProcessor, m_histogramProgressorCompPtr);
};


} // namespace iipr


#endif // !iipr_CColorPatternControllerComp_included


