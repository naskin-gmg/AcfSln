#ifndef iipr_CColorPatternComparatorComp_included
#define iipr_CColorPatternComparatorComp_included


// ACF includes
#include "istd/IInformationProvider.h"
#include "icmm/CHsv.h"
#include "iinsp/TSupplierCompWrap.h"
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
	1. imeas::INumericValueProvider - Get the measured color.
	2. istd::IInformationProvider - Comparison status provider.
*/
class CColorPatternComparatorComp:
			public iinsp::TSupplierCompWrap<imeas::CSimpleNumericValue>,
			virtual public imeas::INumericValueProvider,
			virtual public istd::IInformationProvider
{
public:
	typedef iinsp::TSupplierCompWrap<imeas::CSimpleNumericValue> BaseClass;

	I_BEGIN_COMPONENT(CColorPatternComparatorComp);
		I_REGISTER_INTERFACE(imeas::INumericValueProvider);
		I_REGISTER_INTERFACE(istd::IInformationProvider);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_ASSIGN(m_taskEnabledIdAttrPtr, "TaskEnabledId", "ID of the task enabled attribute", true, "CheckEnabled");
		I_ASSIGN(m_patternCompareThresholdParamIdAttrPtr, "PatternCompareThresholdParamId", "ID of the threshold parameter for pattern comparison", true, "PatternCompareThresholdParamId");
		I_ASSIGN(m_workingPatternProviderCompPtr, "WorkingPatternProvider", "Provider of the working pattern to be compared with the taught one", true, "WorkingPatternProvider");
		I_ASSIGN(m_taughtPatternProviderCompPtr, "TaughtPatternProvider", "Provider of the learned pattern", true, "TaughtPatternProvider");
		I_ASSIGN_TO(m_taughtPatternProviderModelCompPtr, m_taughtPatternProviderCompPtr, false);
		I_ASSIGN_TO(m_taughtPatternSupplierCompPtr, m_taughtPatternProviderCompPtr, false);
		I_ASSIGN(m_dataStatisticsProcessorCompPtr, "DataStatisticsProcessor", "Processor used for calculation of the histogram statistics", true, "DataStatisticsProcessor");
		I_ASSIGN(m_defaultSourceAttrPtr, "DefaultSource", "Default source of information", true, "Color Check");
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
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	bool GetHsvColorValue(const imeas::IDataSequenceStatistics& statistics, icmm::CHsv& hsv) const;

private:
	I_ATTR(QByteArray, m_taskEnabledIdAttrPtr);

	I_ATTR(QByteArray, m_patternCompareThresholdParamIdAttrPtr);
	I_REF(imeas::IDataSequenceProvider, m_workingPatternProviderCompPtr);
	I_REF(imod::IModel, m_workingPatternProviderModelCompPtr);
	I_REF(iinsp::ISupplier, m_workingPatternSupplierCompPtr);

	I_REF(imeas::IDataSequenceProvider, m_taughtPatternProviderCompPtr);
	I_REF(imod::IModel, m_taughtPatternProviderModelCompPtr);
	I_REF(iinsp::ISupplier, m_taughtPatternSupplierCompPtr);

	I_REF(imeas::IDataSequenceStatisticsProcessor, m_dataStatisticsProcessorCompPtr);

	I_ATTR(QString, m_defaultSourceAttrPtr);

	mutable QDateTime m_resultTimeStamp;

	mutable bool m_isColorPatternMatched;
};


} // namespace iipr


#endif // !iipr_CColorPatternComparatorComp_included
