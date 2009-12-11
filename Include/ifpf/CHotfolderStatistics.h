#ifndef ifpf_CHotfolderStatistics_included
#define ifpf_CHotfolderStatistics_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"


// AcfSln includes
#include "ifpf/IHotfolderStatistics.h"
#include "ifpf/IHotfolder.h"


namespace ifpf
{


/**
	Standard implementation if the IHotfolderStatistics interface.
	Statistics data is calculated and updated by observing the changes in the hotfolder state model.
*/
class CHotfolderStatistics: 
			virtual public ifpf::IHotfolderStatistics,
			public imod::TSingleModelObserverBase<ifpf::IHotfolder>
{
public:
	typedef imod::TSingleModelObserverBase<ifpf::IHotfolder> BaseClass;

	CHotfolderStatistics();

	// reimplemented (ifpf::IHotfolderStatistics)
	virtual int GetWaitingCount(const istd::CString& directoryPath = istd::CString()) const;
	virtual int GetProcessedCount(const istd::CString& directoryPath = istd::CString()) const;
	virtual int GetErrorsCount(const istd::CString& directoryPath = istd::CString()) const;
	virtual int GetAbortedCount(const istd::CString& directoryPath = istd::CString()) const;

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual void ResetStatistics();
private:
	int m_waitingCount;
	int m_processedCount;
	int m_errorsCount;
	int m_abortedCount;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderStatistics_included


