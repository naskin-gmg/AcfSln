#pragma once


// ACF-Solutions includes
#include <iipr/CObjectFeature.h>


namespace iipr
{


/**
	Implementation of the pattern search feature.
*/
class CSearchFeature: public CObjectFeature
{
public:
	typedef CObjectFeature BaseClass;

	CSearchFeature();

	CSearchFeature(
				double weight,
				const i2d::CVector2d& position,
				double angle,
				const i2d::CVector2d& scale = i2d::CVector2d(1.0, 1.0),
				bool negativeModel = false,
				const QByteArray& objectId = QByteArray());

	bool IsNegativeModelEnabled() const;
	void SetNegativeModelEnabled(bool enable);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	bool m_isNegativeModelEnabled;
};


} // namespace iipr


