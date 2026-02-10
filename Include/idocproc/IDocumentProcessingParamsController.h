// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>


namespace iprm
{
	class IParamsSet;
}

namespace istd
{
	class IChangeable;
}


namespace idocproc
{


/**
	Interface for configuration of processing parameters/constraints based on the related input data.
*/
class IDocumentProcessingParamsController: virtual public istd::IPolymorphic
{
public:
	/**
		Prepares the processing parameters according to the document, that should be processed.
	*/
	virtual bool ConfigureParametersForDocument(
				const istd::IChangeable& document,
				iprm::IParamsSet& params,
				QString* errorMessagePtr = nullptr) const = 0;
};


} // namespace idocproc


