#ifndef iinsp_CIdProviderCacheComp_included
#define iinsp_CIdProviderCacheComp_included


// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iinsp/IIdProvider.h"


namespace iinsp
{


/**
	General ID provider returning always stored ID.
	Stored ID can be changed only copying their from another ID provider using CopyFrom method.
	This object will be used to manage threading barrier for object supplier chain.
*/
class CIdProviderCacheComp:
			public icomp::CComponentBase,
			virtual public IIdProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CIdProviderCacheComp);
		I_REGISTER_INTERFACE(IIdProvider);
	I_END_COMPONENT;

	CIdProviderCacheComp();

	// reimplemented (iinsp::IIdProvider)
	virtual quint32 GetCurrentId() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	int m_currentId;
};


} // namespace iinsp


#endif // !iinsp_CIdProviderCacheComp_included


