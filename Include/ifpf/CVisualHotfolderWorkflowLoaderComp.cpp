#include "ifpf/CVisualHotfolderWorkflowComp.h"


// Qt includes
#include <QtCore/QMutexLocker>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

// reimplemented (iser::ISerializable)

bool CVisualHotfolderWorkflowComp::Serialize(iser::IArchive& /*archive*/)
{
	bool retVal = true;

	return retVal;
}


} // namespace ifpf


