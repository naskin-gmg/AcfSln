#ifndef ihotf_CVisualHotfolderWorkflowComp_included
#define ihotf_CVisualHotfolderWorkflowComp_included


// ACF includes
#include "istd/TChangeDelegator.h"

#include "iser/IObject.h"

#include "imod/TModelWrap.h"

#include "ibase/TLoggerCompWrap.h"


// AcfSln includes
#include "ihotf/CHotfolderWorkflowComp.h"
#include "ihotf/CVisualHotfolderWorkflowItem.h"


namespace ihotf
{


/**
	
*/
class CVisualHotfolderWorkflowComp:
			public ibase::TLoggerCompWrap<ihotf::CHotfolderWorkflowComp>
{
public:
	typedef ibase::TLoggerCompWrap<ihotf::CHotfolderWorkflowComp> BaseClass;

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

	bool SerializeItemPosition(iser::IArchive& archive, QString& hotfolderName, i2d::CVector2d& position);

	// reimplemented (ihotf::CHotfolderWorkflowComp)
	virtual ihotf::IHotfolderWorkflowItem* CreateWorkflowItem(const QString& hotfolderId) const;
};


} // namespace ihotf


#endif // !ihotf_CVisualHotfolderWorkflowComp_included

