#ifndef iinsp_IConsumer_included
#define iinsp_IConsumer_included


// ACF includes
#include <istd/IChangeable.h>
#include <iinsp/ISupplier.h>


namespace iinsp
{


/**
	Base interface for consumers accepting suppliers.
*/
class IConsumer: virtual public istd::IChangeable
{
public:
	virtual void AcceptSupplier(const ISupplier* supplier) = 0;
};


} // namespace iinsp


#endif // !iinsp_IConsumer_included


