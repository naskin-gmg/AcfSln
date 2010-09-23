#ifndef iqtfpf_CProcessingItemPreviewGuiComp_included
#define iqtfpf_CProcessingItemPreviewGuiComp_included


// ACF includes
#include "imod/IModel.h"
#include "iprm/IFileNameParam.h"

#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingItem.h"
#include "ifpf/IProcessingItemPreviewProvider.h"

#include "iqtfpf/Generated/ui_CProcessingItemPreviewGuiComp.h"


namespace iqtfpf
{


class CProcessingItemPreviewGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CProcessingItemPreviewGuiComp,
						ifpf::IHotfolderProcessingItem>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CProcessingItemPreviewGuiComp,
				ifpf::IHotfolderProcessingItem> BaseClass;

	I_BEGIN_COMPONENT(CProcessingItemPreviewGuiComp)
		I_ASSIGN(m_inputPreviewObserverCompPtr, "InputPreview", "View of the input file", true, "InputPreview");
		I_ASSIGN(m_inputPreviewGuiCompPtr, "InputPreview", "View of the input file", true, "InputPreview");
		I_ASSIGN(m_outputPreviewObserverCompPtr, "OutputPreview", "View of the input file", true, "OutputPreview");
		I_ASSIGN(m_outputPreviewGuiCompPtr, "OutputPreview", "View of the output file", true, "OutputPreview");
		I_ASSIGN(m_inputFileNameParamCompPtr, "InputFileName", "File name parameter for the input file", true, "InputFileName");
		I_ASSIGN(m_inputFileNameParamModelCompPtr, "InputFileName", "File name parameter for the input file", true, "InputFileName");
		I_ASSIGN(m_outputFileNameParamCompPtr, "OutputFileName", "File name parameter for the output file", true, "OutputFileName");
		I_ASSIGN(m_outputFileNameParamModelCompPtr, "OutputFileName", "File name parameter for the output file", true, "OutputFileName");
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(imod::IObserver, m_inputPreviewObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_inputPreviewGuiCompPtr);
	I_REF(imod::IObserver, m_outputPreviewObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_outputPreviewGuiCompPtr);
	I_REF(iprm::IFileNameParam, m_inputFileNameParamCompPtr);
	I_REF(imod::IModel, m_inputFileNameParamModelCompPtr);
	I_REF(iprm::IFileNameParam, m_outputFileNameParamCompPtr);
	I_REF(imod::IModel, m_outputFileNameParamModelCompPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CProcessingItemPreviewGuiComp_included


