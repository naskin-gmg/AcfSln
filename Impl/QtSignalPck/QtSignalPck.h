#ifndef QtSignalPck_included
#define QtSignalPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtsig/CSamplingParamsGuiComp.h"
#include "iqtsig/CSampleAcquisitionGuiComp.h"
#include "iqtsig/CScriptDataProcessorComp.h"
#include "iqtinsp/CInspectionTaskGuiComp.h"
#include "iqtinsp/CGeneralSupplierGuiComp.h"


/**
	Standard Qt package.
*/
namespace QtSignalPck
{


typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;
typedef iqtsig::CScriptDataProcessorComp ScriptDataProcessor;


} // namespace QtSignalPck


#endif // !QtSignalPck_included


