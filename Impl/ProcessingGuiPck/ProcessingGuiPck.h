// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

// ACF-Solutions includes
#include <iprocgui/CProcessorControlGuiComp.h>
#include <iprocgui/CDocumentProcessingManagerComp.h>
#include <iprocgui/CDocumentProcessingCommandComp.h>
#include <iprocgui/CProcessorCommandComp.h>


/**
	Processing GUI package.
*/
namespace ProcessingGuiPck
{


typedef iprocgui::CProcessorControlGuiComp ProcessorControlGui;
typedef icomp::TModelCompWrap<iprocgui::CDocumentProcessingManagerComp> DocumentProcessingManager;
typedef icomp::TModelCompWrap<iprocgui::CDocumentProcessingCommandComp> DocumentProcessingCommand;
typedef icomp::TModelCompWrap < iprocgui::CProcessorCommandComp> ProcessorCommand;


} // namespace ProcessingGuiPck


