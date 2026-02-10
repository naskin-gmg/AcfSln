// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CCaliperDistanceFeature.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iipr
{


CCaliperDistanceFeature::CCaliperDistanceFeature(const i2d::CLine2d& caliperLine, double weight)
	:BaseClass(weight)
{
	SetPoint1(caliperLine.GetPoint1());
	SetPoint2(caliperLine.GetPoint2());

	imath::CVarVector values;
	values.SetElementsCount(2);

	values.SetElement(0, weight);
	values.SetElement(1, GetDistance());

	SetValues(values);
}


double CCaliperDistanceFeature::GetDistance() const
{
	return GetLength();
}


// reimplemented (imeas::INumericValue)

bool CCaliperDistanceFeature::IsValueTypeSupported(ValueTypeId valueTypeId) const
{
	switch (valueTypeId){
		case VTI_AUTO:
		case VTI_WEIGHT:
		case VTI_LENGTH:
			return true;
		default:
			break;
	}

	return false;
}


imath::CVarVector CCaliperDistanceFeature::GetComponentValue(ValueTypeId valueTypeId) const
{
	imath::CVarVector retVal;

	switch (valueTypeId){
		case VTI_AUTO:
			return GetValues();

		case VTI_WEIGHT:
			retVal.SetElementsCount(1);
			retVal.SetElement(0, GetWeight());
			break;

		case VTI_LENGTH:
			retVal.SetElementsCount(1);
			retVal.SetElement(0, GetDistance());
			break;
		default:
			break;
	}

	return retVal;
}


} // namespace iipr


