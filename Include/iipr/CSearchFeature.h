#ifndef iipr_CSearchFeature_included
#define iipr_CSearchFeature_included


// ACF includes
#include "i2d/CVector2d.h"
#include "i2d/CPosition2d.h"
#include "i2d/CAffineTransformation2d.h"

// ACF-Solutions includes
#include "iipr/TWeightedFeatureWrap.h"


namespace iipr
{


/**
	Implementation of the pattern search feature.
*/
class CSearchFeature: public TWeightedFeatureWrap<i2d::CPosition2d>
{
public:
	typedef TWeightedFeatureWrap<i2d::CPosition2d> BaseClass;

	CSearchFeature(
				double weight,
				const i2d::CVector2d& position,
				double angle,
				const i2d::CVector2d& scale,
				int index = -1,
				const QString& id = "");

	double GetAngle() const;
	const i2d::CVector2d& GetScale() const;
	int GetIndex() const;
	const QString& GetId() const;
	const i2d::ITransformation2d& GetTransformation() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_angle;
	i2d::CVector2d m_scale;
	int m_index;
	QString m_id;
	mutable i2d::CAffineTransformation2d m_transformation;
};


} // namespace iipr


#endif // !iipr_CSearchFeature_included


