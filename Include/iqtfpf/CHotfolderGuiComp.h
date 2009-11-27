#ifndef iqtfpf_CHotfolderGuiComp_included
#define iqtfpf_CHotfolderGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"

#include "iqtfpf/Generated/ui_CHotfolderGuiComp.h"


namespace iqtfpf
{


class CHotfolderGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CHotfolderGuiComp,
			ifpf::IHotfolder>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CHotfolderGuiComp,
				ifpf::IHotfolder> BaseClass;

	I_BEGIN_COMPONENT(CHotfolderGuiComp)
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderGuiComp_included


