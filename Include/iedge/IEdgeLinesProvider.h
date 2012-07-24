#ifndef iedge_IEdgeLinesProvider_included
#define iedge_IEdgeLinesProvider_included


// ACF includes
#include "istd/IChangeable.h"

// QSF includes
#include "iedge/CEdgeLine.h"


namespace iedge
{


class IEdgeLinesProvider: public istd::IChangeable
{
public:
	/**
		Provide access to container of endges.
		\return contour container containing set of contours or NULL if no contours are available.
	*/
	virtual const CEdgeLine::Container* GetEdgesContainer() const = 0;
};


} // namespace iedge


#endif // !iedge_IEdgeLinesProvider_included


