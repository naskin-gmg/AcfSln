#include "iipr/CBlobFeature.h"


// ACF includes
#include "istd/TDelPtr.h"
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


// public methods

CBlobFeature::CBlobFeature()
	:m_pixelCount(0),
	m_perimeter(0)
{
}


CBlobFeature::CBlobFeature(
			int pixelCount,
			int perimeter,
			const i2d::CVector2d& position,
			double angle)
	:BaseClass(1.0, position, angle),
	m_pixelCount(pixelCount),
	m_perimeter(perimeter)
{
}


double CBlobFeature::GetCircularity() const
{
	if (qFuzzyCompare(m_perimeter, 0.0))
	{
		return 0.0;
	}

	return 4 * I_PI * m_pixelCount / double((m_perimeter * m_perimeter));
}


// reimplemented (iser::ISerializable)

bool CBlobFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	static iser::CArchiveTag pixelCountTag("PixelCount", "Number of blob's pixels", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(pixelCountTag);
	retVal = retVal && archive.Process(m_pixelCount);
	retVal = retVal && archive.EndTag(pixelCountTag);

	static iser::CArchiveTag perimeterTag("Perimeter", "Number of pixels that surround object", iser::CArchiveTag::TT_LEAF);
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

		m_pixelCount = objectPtr->m_pixelCount;
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


} // namespace iipr


