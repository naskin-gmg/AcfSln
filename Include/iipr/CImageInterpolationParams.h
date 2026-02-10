// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CImageInterpolationParams_included
#define iipr_CImageInterpolationParams_included


// ACF-Solutions includes
#include <iipr/IImageInterpolationParams.h>


namespace iipr
{


/**
	Common interace for interpolation parameters
*/
class CImageInterpolationParams: virtual public iipr::IImageInterpolationParams
{
public:
	CImageInterpolationParams();

	// reimplemented (iipr::IImageInterpolationParams)
	virtual int GetInterpolationMode() const override;
	virtual void SetInterpolationMode(int interpolationMode) override;
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	int m_interpolationMode;
};


} // namespace iipr


#endif // !iipr_CImageInterpolationParams_included


