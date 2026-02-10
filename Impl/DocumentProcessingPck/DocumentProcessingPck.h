// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef DocumentProcessingPck_included
#define DocumentProcessingPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <idocproc/CRenderedDocumentPreviewGeneratorComp.h>


/**
	Document processing package
*/
namespace DocumentProcessingPck
{


typedef icomp::TModelCompWrap<
			idocproc::CRenderedDocumentPreviewGeneratorComp> RenderedDocumentPreviewGenerator;


} // namespace DocumentProcessingPck


#endif // !DocumentProcessingPck_included


