#ifndef iinsp_CIdCacheComp_included
#define iinsp_CIdCacheComp_included


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
class CIdCacheComp:
			public icomp::CComponentBase,
			virtual public IIdProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CIdCacheComp);
		I_REGISTER_INTERFACE(IIdProvider);
	I_END_COMPONENT;

	CIdCacheComp();

	// reimplemented (iinsp::IIdProvider)
	virtual quint32 GetCurrentId() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

private:
	int m_currentId;
};


} // namespace iinsp


#endif // !iinsp_CIdCacheComp_included


