// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_IFeatureToImageMapper_included
#define iipr_IFeatureToImageMapper_included


// ACF includes
#include <i2d/CVector2d.h>
#include <iprm/IParamsSet.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>


namespace iipr
{


/**
	Interface allowing mapping of extracted features to native coordinate systems.
*/
class IFeatureToImageMapper: virtual public istd::IPolymorphic
{
public:
	/**
		Get position in image coordination system using position extracted from projection.
	*/
	virtual bool GetImagePosition(
				const imeas::INumericValue& feature,
				const iprm::IParamsSet* paramsPtr,
				i2d::CVector2d& result) const = 0;
};


} // namespace iipr


#endif // !iipr_IFeatureToImageMapper_included


