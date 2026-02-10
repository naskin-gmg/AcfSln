// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <icalib/CLUTCalibration2d.h>


namespace icalib
{


/**
	Component for lookuptable based 2D calibration.
*/
class CLUTCalibration2dComp : public icomp::CComponentBase, public CLUTCalibration2d
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CLUTCalibration2d BaseClass2;

	I_BEGIN_COMPONENT(CLUTCalibration2dComp);
		I_REGISTER_INTERFACE(ICalibration2d);
		I_REGISTER_INTERFACE(ITransformation2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(CLUTCalibration2d);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(imath::IUnitInfo, m_argumentUnitInfoCompPtr);
	I_REF(imath::IUnitInfo, m_resultUnitInfoCompPtr);
};


} // namespace icalib


