#include "ifpf/CHotfolderWorkflowItem.h"


// Qt includes
#include <QtCore/QStringList>


namespace ifpf
{


// public methods

QString CHotfolderWorkflowItem::GetHotfolderId() const
{
	return m_hotfolderId;
}


void CHotfolderWorkflowItem::Initialize(
			const QString& hotfolderId,
			const ifpf::IHotfolderInfoManager* infoManagerPtr)
{
	m_hotfolderId = hotfolderId;

	m_infoManagerPtr = infoManagerPtr;
}


// reimplemented (ifpf::IHotfolderWorkflowItem)

QStringList CHotfolderWorkflowItem::GetInputDirectories() const
{
	QStringList retVal;

	return retVal;
}


QString CHotfolderWorkflowItem::GetOutputDirectory() const
{
	QString retVal;

	return retVal;
}


// reimplemented (iser::ISerializable)

bool CHotfolderWorkflowItem::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


} // namespace ifpf



