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

	CSearchFeature();

	CSearchFeature(
				double weight,
				const i2d::CVector2d& position,
				double angle,
				const i2d::CVector2d& scale,
				bool negativeModel = false,
				int index = -1,
				const QString& id = "");

	double GetAngle() const;
	const i2d::CVector2d& GetScale() const;
	int GetIndex() const;
	const QString& GetId() const;
	void SetId(QString id);
	bool IsNegativeModelEnabled() const;
	void SetNegativeModelEnabled(bool enable);
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
	i2d::CVector2d m_scale;
	double m_angle;
	int m_index;
	QString m_id;
	mutable i2d::CAffineTransformation2d m_transformation;
	bool m_isNegativeModelEnabled;
};


} // namespace iipr


#endif // !iipr_CSearchFeature_included


