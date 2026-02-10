// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icalib/CAffineCalibration2dComp.h>


// ACF incldues
#include <istd/CChangeGroup.h>


namespace icalib
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CAffineCalibration2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	i2d::CVector2d& translationRef = m_transformation.GetTranslationRef();
	i2d::CMatrix2d& matrixRef = m_transformation.GetDeformMatrixRef();

	if (m_m00AttrPtr.IsValid()){
		matrixRef.GetAtRef(0, 0) = *m_m00AttrPtr;
	}

	if (m_m10AttrPtr.IsValid()){
		matrixRef.GetAtRef(1, 0) = *m_m10AttrPtr;
	}

	if (m_m01AttrPtr.IsValid()){
		matrixRef.GetAtRef(0, 1) = *m_m01AttrPtr;
	}

	if (m_m11AttrPtr.IsValid()){
		matrixRef.GetAtRef(1, 1) = *m_m11AttrPtr;
	}

	if (m_translationXAttrPtr.IsValid()){
		translationRef.SetX(*m_translationXAttrPtr);
	}

	if (m_translationYAttrPtr.IsValid()){
		translationRef.SetY(*m_translationYAttrPtr);
	}

	SetArgumentUnitInfo(m_argumentUnitInfoCompPtr.GetPtr());
	SetResultUnitInfo(m_resultUnitInfoCompPtr.GetPtr());
}


void CAffineCalibration2dComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	SetArgumentUnitInfo(NULL);
	SetResultUnitInfo(NULL);
}


} // namespace icalib


