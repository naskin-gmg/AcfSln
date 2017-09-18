#ifndef QtSignalPck_included
#define QtSignalPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iqtsig/CSamplingParamsGuiComp.h>
#include <iqtsig/CPulseParamsGuiComp.h>
#include <iqtsig/CTriggerParamsGuiComp.h>
#include <iqtsig/CSampleAcquisitionGuiComp.h>


/**
	Standard Qt package.
*/
namespace QtSignalPck
{


typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CPulseParamsGuiComp PulseParamsGui;
typedef iqtsig::CTriggerParamsGuiComp TriggerParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;


} // namespace QtSignalPck


#endif // !QtSignalPck_included


