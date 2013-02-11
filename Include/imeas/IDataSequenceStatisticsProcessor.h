#ifndef imeas_IDataSequenceStatisticsProcessor_included
#define imeas_IDataSequenceStatisticsProcessor_included


// ACF includes
#include "iproc/IProcessor.h"


namespace imeas
{


class IDataSequence;
class IDataSequenceStatistics;


/**
	Processor interface for statistics calculation of a data sequence.
*/
class IDataSequenceStatisticsProcessor: virtual public iproc::IProcessor
{
public:
	/**
		Calculate statistics of a data sequence.
	*/
	virtual int CalculateDataStatistics(
				const imeas::IDataSequence& dataSequence,
				imeas::IDataSequenceStatistics& dataStatistics) const = 0;
};


} // namespace imeas


#endif // !imeas_IDataSequenceStatisticsProcessor_included


