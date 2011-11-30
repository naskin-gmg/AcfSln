#ifndef iinsp_CIdCacheComp_included
#define iinsp_CIdCacheComp_included


// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iinsp/IIdProvider.h"


namespace iinsp
{


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
	virtual I_DWORD GetCurrentId() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

private:
	int m_currentId;
};


} // namespace iinsp


#endif // !iinsp_CIdCacheComp_included


