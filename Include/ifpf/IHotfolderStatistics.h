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
	virtual int GetWaitingCount(const istd::CString& directoryPath = istd::CString()) const = 0;
	virtual int GetProcessedCount(const istd::CString& directoryPath = istd::CString()) const = 0;
	virtual int GetErrorsCount(const istd::CString& directoryPath = istd::CString()) const = 0;
	virtual int GetAbortedCount(const istd::CString& directoryPath = istd::CString()) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderStatistics_included


