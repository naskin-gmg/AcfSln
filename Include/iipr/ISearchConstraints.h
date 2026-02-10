// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_IISearchParamsConstraints_included
#define iipr_IISearchParamsConstraints_included


#include <istd/IChangeable.h>
#include <istd/TRange.h>


namespace iipr
{


/**	
	Interface for search algorithm limitations.
*/
class ISearchConstraints: virtual public istd::IChangeable
{
public:
	/**
		List of flags for supported options.
		This flags can be binary combined.
	*/
	enum SearchSupportedFlags
	{
		SSF_NONE = 0,
		/**
			Translation is supported by model search.
		*/
		SSF_TRANSLATION = 1 << 0,
		/**
			Rotation is supported by model search.
		*/
		SSF_ROTATION = 1 << 1,
		/**
			Scaling is supported by model search.
		*/
		SSF_SCALING = 1 << 2,
		/**
			More than one result is supported.
		*/
		SSF_MULTIPLE_RESULTS = 1 << 3,
		/**
			Quality score threshold is supported.
		*/
		SSF_SCORE_THRESHOLD = 1 << 4
	};

	/**
		Get set of supported options.
	*/
	virtual int GetSearchSupportedFlags() const = 0;
	/**
		Get boundaries of rotation range.
	*/
	virtual const istd::CRange& GetRotationRangeConstraints() const = 0;
	/**
		Get boundaries of scaling range.
	*/
	virtual const istd::CRange& GetScaleRangeConstraints() const = 0;
	/**
		Get boundaries of result count.
	*/
	virtual const istd::CIntRange& GetResultsCountConstraints() const = 0;
};


} // namespace iipr


#endif // !iipr_IISearchParamsConstraints_included

