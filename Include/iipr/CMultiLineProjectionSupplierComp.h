// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CMultiLineProjectionSupplierComp_included
#define iipr_CMultiLineProjectionSupplierComp_included


// ACF-Solutions includes
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <iimg/IBitmapProvider.h>
#include <iipr/ILineProjectionProcessor.h>
#include <imeas/IMultiDataSequenceProvider.h>
#include <iipr/IFeaturesProvider.h>
#include <imeas/CGeneralDataSequence.h>


namespace iipr
{


/**
	Provider of bitmap projection series, which are calculated based on list of 2D-lines
*/
class CMultiLineProjectionSupplierComp:
			public iinsp::TSupplierCompWrap< QVector<imeas::CGeneralDataSequence> >,
			virtual public imeas::IMultiDataSequenceProvider,
			virtual public imeas::IDataSequenceProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QVector<imeas::CGeneralDataSequence> > BaseClass;

	I_BEGIN_COMPONENT(CMultiLineProjectionSupplierComp);
		I_REGISTER_INTERFACE(imeas::IMultiDataSequenceProvider);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provides image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_linesProviderCompPtr, "LinesProvider", "Provides set of line projections", true, "LinesProvider");
		I_ASSIGN_TO(m_linesSupplierCompPtr, m_linesProviderCompPtr, false);
		I_ASSIGN_TO(m_linesProviderModelCompPtr, m_linesProviderCompPtr, false);
		I_ASSIGN(m_projectionProcessorCompPtr, "ProjectionProcessor", "Processor for projection data generation", true, "ProjectionProcessor");
	I_END_COMPONENT;

	// reimplemented (imeas::IMultiDataSequenceProvider)
	virtual int GetSequencesCount() const;
	virtual const imeas::IDataSequence* GetDataSequence(int index) const;

	// reimplemented (imeas::IDataSequenceProvider)
	virtual const imeas::IDataSequence* GetDataSequence() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iipr::IFeaturesProvider, m_linesProviderCompPtr);
	I_REF(iinsp::ISupplier, m_linesSupplierCompPtr);
	I_REF(imod::IModel, m_linesProviderModelCompPtr);
	I_REF(iipr::ILineProjectionProcessor, m_projectionProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CMultiLineProjectionSupplierComp_included
