#ifndef iinsp_CGeneralResultCacheComp_included
#define iinsp_CGeneralResultCacheComp_included


// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iinsp/IGeneralResultProvider.h"


namespace iinsp
{


/**
	General result provider returning always stored result state.
	Stored result state can be changed only copying their from another result provider using CopyFrom method.
	This object will be used to manage threading barrier for object supplier chain.
*/
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
	virtual istd::IInformation::InformationCategory GetGeneralResult() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

private:
	istd::IInformation::InformationCategory m_result;
};


} // namespace iinsp


#endif // !iinsp_CGeneralResultCacheComp_included


