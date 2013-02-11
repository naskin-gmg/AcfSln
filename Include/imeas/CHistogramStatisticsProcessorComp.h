#ifndef imeas_CHistogramStatisticsProcessorComp_included
#define imeas_CHistogramStatisticsProcessorComp_included


// ACF includes
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "imeas/IDataSequenceStatisticsProcessor.h"


namespace imeas
{


class IDataSequence;
class IDataSequenceStatistics;
class IDataStatistics;


/**	
	Implementation of a processor for the image histogram calculation.
*/
class CHistogramStatisticsProcessorComp: public iproc::TSyncProcessorCompBase<IDataSequenceStatisticsProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<IDataSequenceStatisticsProcessor> BaseClass;
	
	I_BEGIN_COMPONENT(CHistogramStatisticsProcessorComp);
		I_REGISTER_INTERFACE(IDataSequenceStatisticsProcessor);
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (IDataSequenceStatisticsProcessor)
	virtual int CalculateDataStatistics(
				const imeas::IDataSequence& dataSequence,
				imeas::IDataSequenceStatistics& dataStatistics) const;

private:
	bool CalculateChannelStatistics(const imeas::IDataSequence& input, int inputIndex, imeas::IDataStatistics& dataStatistics) const;
};


} // namespace imeas


#endif // !imeas_CHistogramStatisticsProcessorComp_included

