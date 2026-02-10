// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CImageInterpolationParamsComp_included
#define iipr_CImageInterpolationParamsComp_included


// ACF includes
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <iipr/CImageInterpolationParams.h>


namespace iipr
{


class CImageInterpolationParamsComp: public icomp::CComponentBase, public CImageInterpolationParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CImageInterpolationParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IImageInterpolationParams);
		I_ASSIGN(m_interpolationModeAttrPtr, "InterpolationMode", "Interpolation mode:\n0 - no interpolation\n1 - bilinear\n2 - bicubic", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(int, m_interpolationModeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImageInterpolationParamsComp_included


