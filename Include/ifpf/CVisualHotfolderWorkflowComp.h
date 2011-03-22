#ifndef ifpf_CVisualHotfolderWorkflowComp_included
#define ifpf_CVisualHotfolderWorkflowComp_included


// ACF includes
#include "istd/TChangeDelegator.h"

#include "iser/IObject.h"

#include "imod/TModelWrap.h"

#include "ibase/TLoggerCompWrap.h"


// AcfSln includes
#include "ifpf/CHotfolderWorkflowComp.h"
#include "ifpf/CVisualHotfolderWorkflowItem.h"


namespace ifpf
{


/**
	
*/
class CVisualHotfolderWorkflowComp:
			public ibase::TLoggerCompWrap<ifpf::CHotfolderWorkflowComp>
{
public:
	typedef ibase::TLoggerCompWrap<ifpf::CHotfolderWorkflowComp> BaseClass;

	I_BEGIN_COMPONENT(CVisualHotfolderWorkflowComp);
	I_END_COMPONENT;

	enum ChangeFlags
	{
		CF_SELECTION = 0x10000000
	};

	enum MessageId
	{
		MI_CANNOT_CREATE_ELEMENT = 650
	};

	virtual bool SerializeLayout(iser::IArchive& archive);
	virtual bool SerializeWorkflow(iser::IArchive& archive);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef imod::TModelWrap<istd::TChangeDelegator<CVisualHotfolderWorkflowItem> > Element;

	bool SerializeItemPosition(iser::IArchive& archive, istd::CString& hotfolderName, i2d::CVector2d& position);

	// reimplemented (ifpf::CHotfolderWorkflowComp)
	virtual ifpf::IHotfolderWorkflowItem* CreateWorkflowItem(const istd::CString& hotfolderId) const;
};


} // namespace ifpf


#endif // !ifpf_CVisualHotfolderWorkflowComp_included

