// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iblob/CBlobFeature.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iblob
{


// public methods

CBlobFeature::CBlobFeature()
			:BaseClass(),
			m_area(0.0),
			m_perimeter(0.0)
{
}


CBlobFeature::CBlobFeature(
			double area,
			double perimeter,
			const i2d::CVector2d& position,
			double angle,
			const i2d::CVector2d& scale,
			const i2d::CPolygon& contour)
	:BaseClass(area, position, angle, scale),
	m_area(area),
	m_perimeter(perimeter),
	m_contour(contour)
{
	if (!m_contour.IsEmpty()){
		m_area = m_contour.GetArea();

		m_perimeter = m_contour.GetPerimeter();
	}
}


double CBlobFeature::GetCircularity() const
{
	return 4 * I_PI * GetCompactness();
}


double CBlobFeature::GetCompactness() const
{
	if (qFuzzyCompare(m_perimeter, 0.0))
		return 0.0;
	else
		return m_area / (m_perimeter * m_perimeter);
}


double CBlobFeature::GetPerimeter() const
{
	return m_perimeter;
}


double CBlobFeature::GetArea() const
{
	return m_area;
}


const i2d::CPolygon& CBlobFeature::GetContour() const
{
	return m_contour;
}


// reimplemented (iser::ISerializable)

bool CBlobFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	istd::CChangeNotifier notifier(!archive.IsStoring() ? this : NULL);

	retVal = retVal && BaseClass::Serialize(archive);

	static iser::CArchiveTag areaTag("Area", "Area of the found blob", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(areaTag);
	retVal = retVal && archive.Process(m_area);
	retVal = retVal && archive.EndTag(areaTag);

	static iser::CArchiveTag perimeterTag("Perimeter", "Length of the object's contour", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(perimeterTag);
	retVal = retVal && archive.Process(m_perimeter);
	retVal = retVal && archive.EndTag(perimeterTag);

	static iser::CArchiveTag contourTag("Contour", "Blob contour", iser::CArchiveTag::TT_GROUP);
	retVal = retVal && archive.BeginTag(contourTag);
	retVal = retVal && m_contour.Serialize(archive);
	retVal = retVal && archive.EndTag(contourTag);


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
		m_contour.CopyFrom(objectPtr->m_contour, mode);

		BaseClass::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeableUniquePtr CBlobFeature::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr retVal(new CBlobFeature);

	if (retVal->CopyFrom(*this, mode)){
		return retVal;
	}

	return NULL;
}


// reimplemented (i2d::IObject2d)

void CBlobFeature::SetCalibration(const i2d::ICalibration2d* calibrationPtr, bool releaseFlag)
{
	BaseClass::SetCalibration(calibrationPtr, releaseFlag);

	m_contour.SetCalibration(calibrationPtr, releaseFlag);
}


i2d::CRectangle CBlobFeature::GetBoundingBox() const
{
	return m_contour.GetBoundingBox();
}


bool CBlobFeature::Transform(
	const i2d::ITransformation2d& transformation,
	i2d::ITransformation2d::ExactnessMode mode,
	double* errorFactorPtr)
{
	BaseClass::Transform(transformation, mode, errorFactorPtr);

	return m_contour.Transform(transformation, mode, errorFactorPtr);
}


bool CBlobFeature::InvTransform(
	const i2d::ITransformation2d& transformation,
	i2d::ITransformation2d::ExactnessMode mode,
	double* errorFactorPtr)
{
	BaseClass::InvTransform(transformation, mode, errorFactorPtr);

	return m_contour.InvTransform(transformation, mode, errorFactorPtr);
}


} // namespace iblob


