#ifndef iedge_IEdgeLinesProvider_included
#define iedge_IEdgeLinesProvider_included


// ACF includes
#include <istd/IChangeable.h>

// ACF-Solutions includes
#include <iedge/CEdgeLineContainer.h>


namespace iedge
{


class IEdgeLinesProvider: virtual public istd::IChangeable
{
public:
	/**
		Provide access to container of endges.
		\return contour container containing set of contours or NULL if no contours are available.
	*/
	virtual const CEdgeLineContainer* GetEdgesContainer() const = 0;
};


} // namespace iedge


#endif // !iedge_IEdgeLinesProvider_included


