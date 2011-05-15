#ifndef imeas_CHistogramStatisticsProcessorComp_included
#define imeas_CHistogramStatisticsProcessorComp_included


// ACF includes
#include "iproc/TSyncProcessorCompBase.h"


namespace imeas
{


class IDataSequence;
class IDataSequenceStatistics;
class IDataStatistics;


/**	
	Implementation of a processor for the image histogram calculation.
*/
class CHistogramStatisticsProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CHistogramStatisticsProcessorComp);
	I_END_COMPONENT

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	bool CalculateHistogramStatistics(const imeas::IDataSequence& input, imeas::IDataSequenceStatistics& histogramStatistics) const;
	bool CalculateChannelStatistics(const imeas::IDataSequence& input, int inputIndex, imeas::IDataStatistics& dataStatistics) const;
};


} // namespace imeas


#endif // !imeas_CHistogramStatisticsProcessorComp_included

