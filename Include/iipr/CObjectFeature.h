#pragma once


// ACF includes
#include "i2d/CPosition2d.h"
#include "i2d/CAffineTransformation2d.h"

// ACF-Solutions includes
#include "iipr/TWeightedFeatureWrap.h"


namespace iipr
{


/**
	Implementation of a general image object feature.
*/
class CObjectFeature: public TWeightedFeatureWrap<i2d::CPosition2d>
{
public:
	typedef TWeightedFeatureWrap<i2d::CPosition2d> BaseClass;

	CObjectFeature();
	CObjectFeature(
				double weight,
				const i2d::CVector2d& position,
				double angle = 0.0,
				const i2d::CVector2d& scale = i2d::CVector2d(1.0, 1.0));

	double GetAngle() const;
	i2d::CVector2d GetScale() const;

	QByteArray GetObjectId() const;
	void SetObjectId(const QByteArray& objectId);

	const i2d::ITransformation2d& GetTransformation() const;

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	double m_angle;
	i2d::CVector2d m_scale;
	QByteArray m_objectId;
	mutable i2d::CAffineTransformation2d m_transformation;
};


} // namespace iipr


