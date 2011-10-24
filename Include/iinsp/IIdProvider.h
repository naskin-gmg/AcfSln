#ifndef iinsp_IIdProvider_included
#define iinsp_IIdProvider_included


// ACF includes
#include "istd/IChangeable.h"


namespace iinsp
{


/**
	Allow access to some ID.
	The meaning of the ID is depend on use case and implementation.
	Typically ID is used to unique identify inspection objects.
*/
class IIdProvider: virtual public istd::IChangeable
{
public:
	/**
		Get current ID value.
	*/
	virtual I_DWORD GetCurrentId() const = 0;

	/**
		Set current ID value.
		\return	true if ID could be set in the implementation.
	*/
	virtual bool SetCurrentId(I_DWORD id) = 0;
};


} // namespace iinsp


#endif // !iinsp_IIdProvider_included


