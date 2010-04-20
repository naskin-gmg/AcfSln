#include "ifpf/CHotfolderWorkflowItem.h"


namespace ifpf
{


// public methods

istd::CString CHotfolderWorkflowItem::GetHotfolderId() const
{
	return m_hotfolderId;
}


void CHotfolderWorkflowItem::Initialize(
			const istd::CString& hotfolderId,
			const ifpf::IHotfolderInfoManager* infoManagerPtr)
{
	m_hotfolderId = hotfolderId;

	m_infoManagerPtr = infoManagerPtr;
}


// reimplemented (ifpf::IHotfolderWorkflowItem)

istd::CStringList CHotfolderWorkflowItem::GetInputDirectories() const
{
	istd::CStringList retVal;

	return retVal;
}


istd::CString CHotfolderWorkflowItem::GetOutputDirectory() const
{
	istd::CString retVal;

	return retVal;
}


// reimplemented (iser::ISerializable)

bool CHotfolderWorkflowItem::Serialize(iser::IArchive& archive)
{
	return true;
}


} // namespace ifpf



