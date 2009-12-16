#ifndef iqtfpf_CProcessingItemGuiComp_included
#define iqtfpf_CProcessingItemGuiComp_included


// ACF includes
#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingItem.h"

#include "iqtfpf/Generated/ui_CProcessingItemGuiComp.h"


namespace iqtfpf
{


class CProcessingItemGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CProcessingItemGuiComp,
						ifpf::IHotfolderProcessingItem>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CProcessingItemGuiComp,
				ifpf::IHotfolderProcessingItem> BaseClass;

	I_BEGIN_COMPONENT(CProcessingItemGuiComp)
		I_ASSIGN(m_inputPreviewObserverCompPtr, "InputPreview", "View of the input file", true, "InputPreview");
		I_ASSIGN(m_inputPreviewGuiCompPtr, "InputPreview", "View of the input file", true, "InputPreview");
		I_ASSIGN(m_outputPreviewObserverCompPtr, "OutputPreview", "View of the input file", true, "OutputPreview");
		I_ASSIGN(m_outputPreviewGuiCompPtr, "OutputPreview", "View of the output file", true, "OutputPreview");
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(imod::IObserver, m_inputPreviewObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_inputPreviewGuiCompPtr);
	I_REF(imod::IObserver, m_outputPreviewObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_outputPreviewGuiCompPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CProcessingItemGuiComp_included


