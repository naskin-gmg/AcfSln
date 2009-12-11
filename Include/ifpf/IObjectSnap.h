#ifndef ifpf_IObjectSnap_included
#define ifpf_IObjectSnap_included


// ACF includes
#include "istd/IChangeable.h"

#include "istd/CIndex2d.h"

#include "iimg/IBitmap.h"


namespace ifpf
{


/**
	Interface for getting a bitmap snap of any object.
*/
class IObjectSnap: virtual public istd::IPolymorphic
{
public:
	/**
		Get bitmap snap for the data object \c data.
	*/
	virtual bool GetSnap(
				const istd::IChangeable& data,
				iimg::IBitmap& objectSnap,
				const istd::CIndex2d& size) = 0;
};


} // namespace ifpf


#endif // !ifpf_IObjectSnap_included


