#ifndef ProcessingGuiPck_included
#define ProcessingGuiPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iprocgui/CProcessorControlGuiComp.h"
#include "iprocgui/CDocumentProcessingManagerComp.h"
#include "iprocgui/CDocumentProcessingCommandComp.h"
#include "iprocgui/CProcessorCommandComp.h"


/**
	Processing GUI package.
*/
namespace ProcessingGuiPck
{


typedef iprocgui::CProcessorControlGuiComp ProcessorControlGui;
typedef iprocgui::CDocumentProcessingManagerComp DocumentProcessingManager;
typedef iprocgui::CDocumentProcessingCommandComp DocumentProcessingCommand;
typedef iprocgui::CProcessorCommandComp ProcessorCommand;


} // namespace ProcessingGuiPck


#endif // !ProcessingGuiPck_included


