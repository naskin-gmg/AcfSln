#pragma once


// ACF includes
#include "i2d/CPosition2d.h"
#include "i2d/CAffineTransformation2d.h"

// ACF-Solutions includes
#include "iipr/CObjectFeature.h"


namespace iipr
{


/**
	Implementation of the blob-based feature.
*/
class CBlobFeature: public iipr::CObjectFeature
{
public:
	typedef iipr::CObjectFeature BaseClass;

	CBlobFeature();
	CBlobFeature(
				int pixelCount,
				int perimeter,
				const i2d::CVector2d& position,
				double angle = 0.0);

	double GetCircularity() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	int m_pixelCount;
	int m_perimeter;
};


} // namespace iipr


