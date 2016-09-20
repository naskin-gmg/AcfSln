#ifndef iprocgui_CDocumentProcessingCommandComp_included
#define iprocgui_CDocumentProcessingCommandComp_included


// ACF includes
#include <iprocgui/CDocumentProcessingManagerCompBase.h>


namespace iprocgui
{


/**
	Component to trigger some processing action for the current document.
*/
class CDocumentProcessingCommandComp: public iprocgui::CDocumentProcessingManagerCompBase
{
public:
	typedef iprocgui::CDocumentProcessingManagerCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CDocumentProcessingCommandComp);
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Result of document processing", true, "OutputData");
		I_ASSIGN(m_outputDataGuiCompPtr, "OutputDataGui", "UI to show the output data object", false, "OutputDataGui");
	I_END_COMPONENT;

protected:
	// reimplemented (iprocgui::CDocumentProcessingManagerCompBase)
	void DoDocumentProcessing(const istd::IChangeable* inputDocumentPtr, const QByteArray& documentTypeId);

private:
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_REF(iqtgui::IGuiObject, m_outputDataGuiCompPtr);
};


} // namespace iproc


#endif // !iprocgui_CDocumentProcessingCommandComp_included
