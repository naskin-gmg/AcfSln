#pragma once


// ACF includes
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <iblob/CBlobFilterParams.h>


namespace iblob
{


class CBlobFilterParamsComp:
			public icomp::CComponentBase,
			public CBlobFilterParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CBlobFilterParamsComp);
		I_REGISTER_INTERFACE(iblob::IBlobFilterParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_supportedPropertiesCompPtr, "SupportedProperties", "Provides list of features supported for filtering", true, "SupportedProperties");
	I_END_COMPONENT;

	// reimplemented (icomp::CComponentBase)
	void OnComponentCreated();

private:
	I_REF(iprm::IOptionsList, m_supportedPropertiesCompPtr);
};


} // namespace iblob


