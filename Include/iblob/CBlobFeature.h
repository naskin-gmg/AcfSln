#pragma once


// ACF includes
#include "i2d/CPosition2d.h"
#include "i2d/CAffineTransformation2d.h"

// ACF-Solutions includes
#include "iipr/CObjectFeature.h"


namespace iblob
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
				double area,
				double perimeter,
				const i2d::CVector2d& position,
				double angle = 0.0);

	double GetCircularity() const;
	double GetPerimeter() const;
	double GetArea() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	double m_area;
	double m_perimeter;
};


} // namespace iblob


