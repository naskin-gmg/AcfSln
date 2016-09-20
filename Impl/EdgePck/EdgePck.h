#ifndef EdgePck_included
#define EdgePck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iedge/CEdgesReductorComp.h>
#include <iedge/CFastEdgesExtractorComp.h>
#include <iedge/CExtractedEdgeLinesSupplierComp.h>
#include <iedge/CProcessedEdgeLinesSupplierComp.h>
#include <iedge/CEdgeBasedFeaturesSupplierComp.h>
#include <iedgegui/CEdgeLinesSupplierEditorComp.h>


/**
	Base system-undependent general package.
*/
namespace EdgePck
{


typedef icomp::TMakeComponentWrap<iedge::CEdgeLineContainer, iedge::CEdgeLineContainer, iser::ISerializable, istd::IChangeable> EdgeLinesContainer;
typedef iedge::CEdgesReductorComp EdgesReductor;
typedef iedge::CFastEdgesExtractorComp FastEdgesExtractor;
typedef icomp::TModelCompWrap<iedge::CExtractedEdgeLinesSupplierComp> ExtractedEdgeLinesSupplier;
typedef icomp::TModelCompWrap<iedge::CProcessedEdgeLinesSupplierComp> ProcessedEdgeLinesSupplier;
typedef icomp::TModelCompWrap<iedge::CEdgeBasedFeaturesSupplierComp> EdgeBasedFeaturesSupplier;
typedef iedgegui::CEdgeLinesSupplierEditorComp EdgeLinesSupplierEditor;


} // namespace EdgePck


#endif // !EdgePck_included


