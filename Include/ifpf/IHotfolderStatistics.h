#ifndef ifpf_IHotfolderStatistics_included
#define ifpf_IHotfolderStatistics_included


// ACF includes
#include "iser/ISerializable.h"


namespace ifpf
{


/**
	Interface for a simple static of the hotfolder processing.
*/
class IHotfolderStatistics: virtual public iser::ISerializable
{
public:
	virtual int GetItemsCount(const QString& directoryPath = QString()) const = 0;
	virtual int GetSuccessCount(const QString& directoryPath = QString()) const = 0;
	virtual int GetErrorsCount(const QString& directoryPath = QString()) const = 0;
	virtual int GetAbortedCount(const QString& directoryPath = QString()) const = 0;
	virtual double GetProcessingTime(const QString& directoryPath = QString()) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderStatistics_included


