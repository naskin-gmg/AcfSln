// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CCaliperFeature_included
#define iipr_CCaliperFeature_included


#include <imeas/CSimpleNumericValue.h>


namespace iipr
{


/**
	Implementation of INumericValue interface for caliper-extracted features.
*/
class CCaliperFeature: public imeas::CSimpleNumericValue
{
public:
	typedef imeas::CSimpleNumericValue BaseClass;

	enum EdgeMode
	{
		EM_FALLING,
		EM_RISING
	};

	CCaliperFeature(
				double weight,
				double position,
				int edgeMode);

	double GetPosition() const;
	double GetWeight() const;
	int GetEdgeMode() const;

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const override;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	double m_position;
	double m_weight;
	int m_edgeMode;
};


} // namespace iipr


#endif // !iipr_CCaliperFeature_included


