#ifndef iipr_CMultiLineProjectionSupplierComp_included
#define iipr_CMultiLineProjectionSupplierComp_included


// ACF-Solutions includes
#include "iproc/TSupplierCompWrap.h"
#include "iipr/IBitmapProvider.h"
#include "iipr/ILineProjectionProcessor.h"
#include "imeas/IMultiDataSequenceProvider.h"
#include "imeas/INumericValueProvider.h"
#include "imeas/CGeneralDataSequence.h"


namespace iipr
{


/**
	Provider of bitmap projection series, which are calculated based on list of 2D-lines
*/
class CMultiLineProjectionSupplierComp:
			public iproc::TSupplierCompWrap< QVector<imeas::CGeneralDataSequence> >,
			virtual public imeas::IMultiDataSequenceProvider
{
public:
	typedef iproc::TSupplierCompWrap< QVector<imeas::CGeneralDataSequence> > BaseClass;

	I_BEGIN_COMPONENT(CMultiLineProjectionSupplierComp);
		I_REGISTER_INTERFACE(imeas::IMultiDataSequenceProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provides image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_linesProviderCompPtr, "LinesProvider", "Provides set of line projections", true, "LinesProvider");
		I_ASSIGN_TO(m_linesProviderModelCompPtr, m_linesProviderCompPtr, false);
		I_ASSIGN(m_projectionProcessorCompPtr, "ProjectionProcessor", "Processor for projection data generation", true, "ProjectionProcessor");
	I_END_COMPONENT;

	// reimplemented (imeas::IMultiDataSequenceProvider)
	virtual int GetSequencesCount() const;
	virtual const imeas::IDataSequence* GetDataSequence(int index) const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iipr::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(imeas::INumericValueProvider, m_linesProviderCompPtr);
	I_REF(imod::IModel, m_linesProviderModelCompPtr);
	I_REF(iipr::ILineProjectionProcessor, m_projectionProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CMultiLineProjectionSupplierComp_included
