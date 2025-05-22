#ifndef iipr_CImageHistogramSupplierComp_included
#define iipr_CImageHistogramSupplierComp_included


// ACF includes
#include <i2d/IMultiCalibrationProvider.h>
#include <i2d/CVector2d.h>
#include <i2d/CAffineTransformation2d.h>
#include <iprm/IParamsSet.h>
#include <istd/IInformationProvider.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <imeas/TDiscreteDataSequence.h>
#include <imeas/IDataSequenceProvider.h>
#include <iimg/IBitmapProvider.h>


namespace iipr
{


class CImageHistogramSupplierComp:
			public iinsp::TSupplierCompWrap< imod::TModelWrap<imeas::CSimpleSamplesSequence32> >,
			virtual public imeas::IDataSequenceProvider
{
public:
	typedef iinsp::TSupplierCompWrap< imod::TModelWrap<imeas::CSimpleSamplesSequence32> > BaseClass;

	I_BEGIN_COMPONENT(CImageHistogramSupplierComp);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_histogramProcessorCompPtr, "HistogramProcessor", "Calculate histogram of the image", true, "HistogramProcessor");
	I_END_COMPONENT;

	// reimplemented (imeas::IDataSequenceProvider)
	virtual const imeas::IDataSequence* GetDataSequence() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(imod::TModelWrap<imeas::CSimpleSamplesSequence32>& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_histogramProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CImageHistogramSupplierComp_included


