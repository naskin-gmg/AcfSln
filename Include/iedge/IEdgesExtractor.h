#ifndef iedge_IEdgesExtractor_included
#define iedge_IEdgesExtractor_included


// ACF includes
#include "iimg/IBitmap.h"
#include "iproc/IProcessor.h"

// QSF includes
#include "iedge/CEdgeLine.h"


namespace iedge
{


/**
	Interface for edge lines extraction from the image.
*/
class IEdgesExtractor: virtual public iproc::IProcessor
{
public:
	/**
		Do extraction of set of edge lines found on the image.
		\param	bitmap		image where the contour lines should be extracted.
		\param	result		container of result edges lines.
		\return				true if success.
	*/
	virtual bool DoContourExtraction(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& bitmap,
				CEdgeLine::Container& result) const = 0;
};


} // namespace iedge


#endif // !iedge_IEdgesExtractor_included


