#include <icomm/CRequestHeader.h>


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>


namespace icomm
{


CRequestHeader::CRequestHeader()
{
	m_requestId = QUuid::createUuid().toByteArray();
}


CRequestHeader::CRequestHeader(const QByteArray& command, const QByteArray& requestId)
:	m_command(command),
	m_requestId(requestId)
{
	if (requestId.isEmpty()){
		m_requestId = QUuid::createUuid().toByteArray();
	}
}


// reimplemented (iser::ISerializable)

bool CRequestHeader::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = true;

	retVal = retVal && archive.Process(m_command);
	retVal = retVal && archive.Process(m_requestId);

	return retVal;
}


} // namespace icomm


