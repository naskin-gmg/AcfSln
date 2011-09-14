#include "ifpf/CVisualHotfolderWorkflowComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/CSectionBlocker.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

// reimplemented (iser::ISerializable)

bool CVisualHotfolderWorkflowComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	return retVal;
}


} // namespace ifpf


