#ifndef iqtfpf_CHotfolderComp_included
#define iqtfpf_CHotfolderComp_included


// Qt includes
#include<QThread>


// ACF includes
#include "ifpf/IDirectoryMonitor.h"

#include "imod/TSingleModelObserverBase.h"

#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IParamsSet.h"


// AcfSln includes
#include "ifpf/IFileNamingStrategy.h"
#include "ifpf/CHotfolderInfo.h"


namespace iqtfpf
{


/**
	Hotfolder representation.
*/
class CHotfolderComp:
			public QThread,
			public ibase::CLoggerComponentBase,
			public imod::TSingleModelObserverBase<ifpf::IDirectoryMonitor>,
			virtual public ifpf::IFileNamingStrategy
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<ifpf::IDirectoryMonitor> BaseClass2;
	typedef QThread BaseClass3;

	I_BEGIN_COMPONENT(CHotfolderComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(ifpf::IFileNamingStrategy);
		I_REGISTER_INTERFACE(ifpf::IHotfolderInfo);
		I_ASSIGN(m_fileConvertCompPtr, "FileConverter", "File converter", true, "FileConverter");
		I_ASSIGN(m_fileNamingStrategyCompPtr, "FileNamingStrategy", "Strategy for naming of the output file", true, "FileNamingStrategy");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameter set for the hotfolder", true, "ParamsSet");
		I_ASSIGN(m_hotfolderPathIdAttrPtr, "HotfolderPathId", "Parameter set ID for the hotfolder directory", true, "");
		I_ASSIGN(m_outputDirectoryIdAttrPtr, "OutputDirectoryId", "Parameter set ID for the output directory", true, "");
	I_END_COMPONENT();

	// reimplemented (ifpf::IFileNamingStrategy)
	virtual istd::CString GetFileName(const istd::CString& inputFileName) const;

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_REF(ibase::IFileConvertCopy, m_fileConvertCompPtr);
	I_REF(ifpf::IFileNamingStrategy, m_fileNamingStrategyCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_ATTR(istd::CString, m_hotfolderPathIdAttrPtr);
	I_ATTR(istd::CString, m_outputDirectoryIdAttrPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderComp_included

