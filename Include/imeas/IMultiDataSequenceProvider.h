// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_IMultiDataSequenceProvider_included
#define imeas_IMultiDataSequenceProvider_included


// ACF includes
#include <imeas/IDataSequenceProvider.h>


namespace imeas
{


/**
	Interface for a provider of more data sequence objects.
*/
class IMultiDataSequenceProvider: virtual public istd::IChangeable
{
public:
	/**
		Get the number of available data sequences.
	*/
	virtual int GetSequencesCount() const = 0;

	/**
		Get data sequence with the given index.
	*/
	virtual const imeas::IDataSequence* GetDataSequence(int index) const = 0;
};


} // namespace imeas


#endif // !imeas_IMultiDataSequenceProvider_included


