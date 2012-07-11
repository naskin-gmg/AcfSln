#ifndef EdgePck_included
#define EdgePck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iedge/CEdgesReductorComp.h"
#include "iedge/CFastEdgesExtractorComp.h"


/**
	Base system-undependent general package.
*/
namespace EdgePck
{


typedef icomp::TMakeComponentWrap<iedge::CEdgeLine::Container, iedge::CEdgeLine::Container, iser::ISerializable, istd::IChangeable> EdgesContainer;
typedef iedge::CEdgesReductorComp EdgesReductor;
typedef iedge::CFastEdgesExtractorComp FastEdgesExtractor;


} // namespace EdgePck


#endif // !EdgePck_included


