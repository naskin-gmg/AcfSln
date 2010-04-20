#ifndef ifpf_CHotfolderWorkflowItem_included
#define ifpf_CHotfolderWorkflowItem_included


// ACF includes
#include "ibase/TNamedWrap.h"


// AcfSln includes
#include "ifpf/IHotfolderWorkflowItem.h"
#include "ifpf/IHotfolderInfoManager.h"


namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolderWorkflowItem: public ibase::TNamedWrap<ifpf::IHotfolderWorkflowItem>
{
public:
	typedef ibase::TNamedWrap<istd::INamed> BaseClass;

	void Initialize(const istd::CString& hotfolderId,
					const ifpf::IHotfolderInfoManager* infoManagerPtr);

	/**
		Get hotfolder's ID
	*/
	virtual istd::CString GetHotfolderId() const;

	// reimplemented (ifpf::IHotfolderWorkflowItem)
	virtual istd::CStringList GetInputDirectories() const;
	virtual istd::CString GetOutputDirectory() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_hotfolderId;

	const ifpf::IHotfolderInfoManager* m_infoManagerPtr;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderWorkflowItem_included

