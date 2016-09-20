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
				const i2d::CVector2d& scale,
				bool negativeModel = false,
				int index = -1,
				const QByteArray& objectId = QByteArray());

	int GetIndex() const;
	bool IsNegativeModelEnabled() const;
	void SetNegativeModelEnabled(bool enable);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	int m_index;
	bool m_isNegativeModelEnabled;
};


} // namespace iipr


