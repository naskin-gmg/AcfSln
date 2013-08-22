#ifndef ihotfgui_CProcessingItemPreviewGuiComp_included
#define ihotfgui_CProcessingItemPreviewGuiComp_included


// ACF includes
#include "imod/IModel.h"
#include "ifile/IFileNameParam.h"

#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ihotf/IHotfolderProcessingItem.h"

#include "GeneratedFiles/ihotfgui/ui_CProcessingItemPreviewGuiComp.h"


namespace ihotfgui
{


class CProcessingItemPreviewGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CProcessingItemPreviewGuiComp,
						ihotf::IHotfolderProcessingItem>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CProcessingItemPreviewGuiComp,
				ihotf::IHotfolderProcessingItem> BaseClass;

	I_BEGIN_COMPONENT(CProcessingItemPreviewGuiComp);
		I_ASSIGN(m_inputPreviewObserverCompPtr, "InputPreview", "View of the input file", true, "InputPreview");
		I_ASSIGN_TO(m_inputPreviewGuiCompPtr, m_inputPreviewObserverCompPtr, true);
		I_ASSIGN(m_outputPreviewObserverCompPtr, "OutputPreview", "View of the input file", true, "OutputPreview");
		I_ASSIGN_TO(m_outputPreviewGuiCompPtr, m_outputPreviewObserverCompPtr, true);
		I_ASSIGN(m_inputFileNameParamCompPtr, "InputFileName", "File name parameter for the input file", true, "InputFileName");
		I_ASSIGN_TO(m_inputFileNameParamModelCompPtr, m_inputFileNameParamCompPtr, true);
		I_ASSIGN(m_outputFileNameParamCompPtr, "OutputFileName", "File name parameter for the output file", true, "OutputFileName");
		I_ASSIGN_TO(m_outputFileNameParamModelCompPtr, m_outputFileNameParamCompPtr, true);
	I_END_COMPONENT;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	void ResetEditor();

private:
	I_REF(imod::IObserver, m_inputPreviewObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_inputPreviewGuiCompPtr);
	I_REF(imod::IObserver, m_outputPreviewObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_outputPreviewGuiCompPtr);
	I_REF(ifile::IFileNameParam, m_inputFileNameParamCompPtr);
	I_REF(imod::IModel, m_inputFileNameParamModelCompPtr);
	I_REF(ifile::IFileNameParam, m_outputFileNameParamCompPtr);
	I_REF(imod::IModel, m_outputFileNameParamModelCompPtr);
};


} // namespace ihotfgui


#endif // !ihotfgui_CProcessingItemPreviewGuiComp_included


