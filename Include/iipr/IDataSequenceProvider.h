#ifndef iipr_IDataSequenceProvider_included
#define iipr_IDataSequenceProvider_included


// ACF includes
#include "istd/IChangeable.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"


namespace iipr
{


/**
	Image line projection supplier.
*/
class IDataSequenceProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to produced line projection object.
	*/
	virtual const imeas::IDataSequence* GetDataSequence() const = 0;
};


} // namespace iipr


#endif // !iipr_IDataSequenceProvider_included


