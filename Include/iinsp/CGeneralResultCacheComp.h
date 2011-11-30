#ifndef iinsp_CGeneralResultCacheComp_included
#define iinsp_CGeneralResultCacheComp_included


// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iinsp/IGeneralResultProvider.h"


namespace iinsp
{


class CGeneralResultCacheComp:
			public icomp::CComponentBase,
			virtual public IGeneralResultProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CGeneralResultCacheComp);
		I_REGISTER_INTERFACE(IGeneralResultProvider);
	I_END_COMPONENT;

	CGeneralResultCacheComp();

	// reimplemented (iinsp::IGeneralResultProvider)
	virtual GeneralResult GetGeneralResult() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

private:
	GeneralResult m_result;
};


} // namespace iinsp


#endif // !iinsp_CGeneralResultCacheComp_included


