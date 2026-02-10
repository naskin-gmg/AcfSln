// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_IDataSequenceProvider_included
#define imeas_IDataSequenceProvider_included


// ACF includes
#include <istd/IChangeable.h>

// ACF-Solutions includes
#include <imeas/IDataSequence.h>


namespace imeas
{


/**
	Simple data sequence provider.
*/
class IDataSequenceProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to produced line projection object.
	*/
	virtual const imeas::IDataSequence* GetDataSequence() const = 0;
};


} // namespace imeas


#endif // !imeas_IDataSequenceProvider_included


