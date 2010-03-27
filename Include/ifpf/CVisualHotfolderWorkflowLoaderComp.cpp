#include "ifpf/CHotfolderWorkflowComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/CSectionBlocker.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

CHotfolderWorkflowComp::CHotfolderWorkflowComp()
{
}


// reimplemented (ifpf::IHotfolderWorkflow)

ifpf::IHotfolderProcessingInfo* CHotfolderWorkflowComp::AddHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId)
{
	return NULL;
}


bool CHotfolderWorkflowComp::RemoveHotfolder(const istd::CString& hotfolderName)
{
	return true;
}


ifpf::IHotfolderProcessingInfo* CHotfolderWorkflowComp::GetHotfolder(const istd::CString& hotfolderName) const
{
	return NULL;
}


istd::CStringList CHotfolderWorkflowComp::GetHotfoldersList() const
{
	return istd::CStringList();
}


int CHotfolderWorkflowComp::GetWorkingState(const istd::CString& hotfolderName) const
{
	return 0;
}


// reimplemented (iser::ISerializable)

bool CHotfolderWorkflowComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	return retVal;
}


} // namespace ifpf


