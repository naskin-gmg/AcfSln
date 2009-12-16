#ifndef iqtfpf_CProcessingItemInfoGuiComp_included
#define iqtfpf_CProcessingItemInfoGuiComp_included


// ACF includes
#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingItem.h"

#include "iqtfpf/Generated/ui_CProcessingItemInfoGuiComp.h"


namespace iqtfpf
{


class CProcessingItemInfoGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CProcessingItemInfoGuiComp,
						ifpf::IHotfolderProcessingItem>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CProcessingItemInfoGuiComp,
				ifpf::IHotfolderProcessingItem> BaseClass;

	I_BEGIN_COMPONENT(CProcessingItemInfoGuiComp)
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);
};


} // namespace iqtfpf


#endif // !iqtfpf_CProcessingItemInfoGuiComp_included


