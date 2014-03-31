#ifndef ihotf_IMonitoringSession_included
#define ihotf_IMonitoringSession_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>

// ACF includes
#include "istd/IChangeable.h"
#include "iser/ISerializable.h"


namespace ihotf
{


/**
	Common interface for a monitoring session for a directory.
	Each directory monitor can persist its monitoring state in a monitoring session, which is provided by IMonitoringSessionManager.
	\sa IMonitoringSessionManager
*/
class IMonitoringSession: virtual public iser::ISerializable
{
public:
	/**
		Map absolute file path to modification time.
	*/
	typedef QMap<QString, QDateTime> FileItems;

	/**
		Get the list of file items.
	*/
	virtual const FileItems& GetFileInfoList() const = 0;

	/**
		Set the file list.
	*/
	virtual void SetFileInfoList(const FileItems& fileList) = 0;
};


} // namespace ihotf


#endif // !ihotf_IMonitoringSession_included


