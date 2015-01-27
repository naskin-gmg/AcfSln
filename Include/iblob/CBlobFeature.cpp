#include "iblob/CBlobFeature.h"


// ACF includes
#include "istd/TDelPtr.h"
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iblob
{


// public methods

CBlobFeature::CBlobFeature()
	:m_area(0.0),
	m_perimeter(0.0)
{
}


CBlobFeature::CBlobFeature(
			double area,
			double perimeter,
			const i2d::CVector2d& position,
			double angle)
	:BaseClass(1.0, position, angle),
	m_area(area),
	m_perimeter(perimeter)
{
}


double CBlobFeature::GetCircularity() const
{
	if (qFuzzyCompare(m_perimeter, 0.0))
	{
		return 0.0;
	}

	return 4 * I_PI * m_area / (m_perimeter * m_perimeter);
}


double CBlobFeature::GetPerimeter() const
{
	return m_perimeter;
}


double CBlobFeature::GetArea() const
{
	return m_area;
}


// reimplemented (iser::ISerializable)

bool CBlobFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	static iser::CArchiveTag areaTag("Area", "Area of the found blob", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(areaTag);
	retVal = retVal && archive.Process(m_area);
	retVal = retVal && archive.EndTag(areaTag);

	static iser::CArchiveTag perimeterTag("Perimeter", "Length of the object's contour", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(perimeterTag);
	retVal = retVal && archive.Process(m_perimeter);
	retVal = retVal && archive.EndTag(perimeterTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CBlobFeature::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CBlobFeature* objectPtr = dynamic_cast<const CBlobFeature*>(&object);
	if (objectPtr != NULL){
		istd::CChangeNotifier notifier(this);

		m_area = objectPtr->m_area;
		m_perimeter = objectPtr->m_perimeter;

		BaseClass::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeable* CBlobFeature::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CBlobFeature> retVal(new CBlobFeature);

	if (retVal->CopyFrom(*this, mode)){
		return retVal.PopPtr();
	}

	return NULL;
}


} // namespace iblob


