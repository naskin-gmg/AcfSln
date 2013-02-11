#ifndef iipr_CColorPatternComparatorComp_included
#define iipr_CColorPatternComparatorComp_included


// ACF includes
#include "istd/IInformationProvider.h"
#include "iproc/TSupplierCompWrap.h"
#include "icmm/CHsv.h"

// ACF-Solutions includes
#include "imeas/IDataSequenceProvider.h"
#include "imeas/IDataSequenceStatisticsProcessor.h"
#include "imeas/INumericValueProvider.h"
#include "imeas/CSimpleNumericValue.h"
#include "imeas/CGeneralDataSequence.h"


namespace iipr
{


/**
	Color comparator based on histogram statistics.

	Implemented interfaces have following meaning:
	1. imeas::INumericValueProvider - Get the measured color
	2. istd::IInformationProvider - Comparison status provider.
*/
class CColorPatternComparatorComp:
			public iproc::TSupplierCompWrap<imeas::CSimpleNumericValue>,
			virtual public imeas::INumericValueProvider,
			virtual public istd::IInformationProvider
{
public:
	typedef iproc::TSupplierCompWrap<imeas::CSimpleNumericValue> BaseClass;

	I_BEGIN_COMPONENT(CColorPatternComparatorComp);
		I_REGISTER_INTERFACE(imeas::INumericValueProvider);
		I_REGISTER_INTERFACE(istd::IInformationProvider);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_ASSIGN(m_patternCompareThresholdParamIdAttrPtr, "PatternCompareThresholdParamId", "ID of the threshold parameter for pattern comparison", true, "PatternCompareThresholdParamId");
		I_ASSIGN(m_workingPatternProviderCompPtr, "WorkingPatternProvider", "Provider of the working pattern to be compared with the teached one", true, "WorkingPatternProvider");
		I_ASSIGN(m_teachedPatternProviderCompPtr, "TeachedPatternProvider", "Provider of the learned pattern", true, "TeachedPatternProvider");
		I_ASSIGN_TO(m_teachedPatternProviderModelCompPtr, m_teachedPatternProviderCompPtr, false);
		I_ASSIGN_TO(m_teachedPatternSupplierCompPtr, m_teachedPatternProviderCompPtr, false);
		I_ASSIGN(m_dataStatisticsProcessorCompPtr, "DataStatisticsProcessor", "Processor used for calculation of the histogram statistics", true, "DataStatisticsProcessor");
	I_END_COMPONENT;

	CColorPatternComparatorComp();

	// reimplemented (imeas::INumericValueProvider)
	virtual int GetValuesCount() const;
	virtual const imeas::INumericValue& GetNumericValue(int index) const;

	// reimplemented (istd::IInformationProvider)
	virtual QDateTime GetInformationTimeStamp() const;
	virtual InformationCategory GetInformationCategory() const;
	virtual int GetInformationId() const;
	virtual QString GetInformationDescription() const;
	virtual QString GetInformationSource() const;
	virtual int GetInformationFlags() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	bool GetHsvColorValue(const imeas::IDataSequenceStatistics& statistics, icmm::CHsv& hsv) const;

private:
	I_ATTR(QByteArray, m_patternCompareThresholdParamIdAttrPtr);
	I_REF(imeas::IDataSequenceProvider, m_workingPatternProviderCompPtr);
	I_REF(imod::IModel, m_workingPatternProviderModelCompPtr);
	I_REF(iproc::ISupplier, m_workingPatternSupplierCompPtr);

	I_REF(imeas::IDataSequenceProvider, m_teachedPatternProviderCompPtr);
	I_REF(imod::IModel, m_teachedPatternProviderModelCompPtr);
	I_REF(iproc::ISupplier, m_teachedPatternSupplierCompPtr);

	I_REF(imeas::IDataSequenceStatisticsProcessor, m_dataStatisticsProcessorCompPtr);

	mutable QDateTime m_resultTimeStamp;

	mutable bool m_isColorPatternMatched;
};


} // namespace iipr


#endif // !iipr_CColorPatternComparatorComp_included
