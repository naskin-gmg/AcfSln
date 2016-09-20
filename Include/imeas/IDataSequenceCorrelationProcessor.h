#ifndef imeas_IDataSequenceCorrelationProcessor_included
#define imeas_IDataSequenceCorrelationProcessor_included


// ACF includes
#include <iproc/IProcessor.h>


namespace imeas
{


class IDataSequence;


/**
	Interface for 1D-data correlation processor. 
*/
class IDataSequenceCorrelationProcessor: virtual public iproc::IProcessor
{
public:
	/**
		Calculate correlation between two data sequences.
	*/
	virtual bool CalculateCorrelation(
				const imeas::IDataSequence& input1,
				const imeas::IDataSequence& input2,
				imeas::IDataSequence& correlation) const = 0;
};


} // namespace imeas


#endif // !imeas_IDataSequenceCorrelationProcessor_included


