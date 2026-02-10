// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CSearchParamsComp_included
#define iipr_CSearchParamsComp_included


// ACF includes
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <iipr/ISearchConstraints.h>
#include <iipr/CSearchParams.h>


namespace iipr
{


class CSearchParamsComp: 
			public icomp::CComponentBase,
			public CSearchParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSearchParamsComp);
		I_REGISTER_INTERFACE(iipr::ISearchParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_searchConstraintsCompPtr, "SearchConstaints", "Search parameters constraints", false, "SearchConstaints");
		I_ASSIGN(m_defaultModelOccuranceAttrPtr, "DefaultModelOccurance", "Default model occurance\n\t-1 - all models,\n\t0 - no models", true, -1);
		I_ASSIGN(m_defaultMinRotationAngleAttrPtr, "DefaultMinRotationAngle", "Default minimum rotation angle in degrees. Must be set together with DefaultMaxRotationAngle", false, -5.0);
		I_ASSIGN(m_defaultMaxRotationAngleAttrPtr, "DefaultMaxRotationAngle", "Default maximum rotation angle in degrees. Must be set together with DefaultMinRotationAngle", false, 5.0);
	I_END_COMPONENT;

	// reimplemented (iipr::ISearchParams)
	virtual const ISearchConstraints* GetSearchConstraints() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_REF(iipr::ISearchConstraints, m_searchConstraintsCompPtr);
	I_ATTR(int, m_defaultModelOccuranceAttrPtr);
	I_ATTR(double, m_defaultMinRotationAngleAttrPtr);
	I_ATTR(double, m_defaultMaxRotationAngleAttrPtr);
};


} // namespace iipr


#endif // !iipr_CSearchParamsComp_included


