#ifndef imeas_CCrossCorrelationProcessorComp_included
#define imeas_CCrossCorrelationProcessorComp_included


// ACF includes
#include <iproc/TSyncProcessorCompBase.h>

// ACF-Solutions includes
#include <imeas/IDataSequenceCorrelationProcessor.h>


namespace imeas
{


/**	
	Implementation of a processor for calculation of cross correlation between two data sequence objects.
*/
class CCrossCorrelationProcessorComp: public iproc::TSyncProcessorCompBase<IDataSequenceCorrelationProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<IDataSequenceCorrelationProcessor> BaseClass;
	
	I_BEGIN_COMPONENT(CCrossCorrelationProcessorComp);
		I_REGISTER_INTERFACE(IDataSequenceCorrelationProcessor);
	I_END_COMPONENT;

	/**
		Calculate cross correlation between two data sequences.
	*/
	virtual bool CalculateCorrelation(
				const imeas::IDataSequence& input1,
				const imeas::IDataSequence& input2,
				imeas::IDataSequence& correlation) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

protected:
	/**
		Calculate mean sample value for a given channel.
	*/
	static double GetSampleAverage(const imeas::IDataSequence& input, int channelIndex);
};


} // namespace imeas


#endif // !imeas_CCrossCorrelationProcessorComp_included

