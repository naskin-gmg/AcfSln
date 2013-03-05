#ifndef iinsp_CInformationProviderCacheComp_included
#define iinsp_CInformationProviderCacheComp_included


// ACF includes
#include "istd/IInformationProvider.h"
#include "icomp/CComponentBase.h"


namespace iinsp
{


/**
	General information provider returning always stored state.
	Stored result state can be changed only copying their from another result provider using CopyFrom method.
	This object will be used to manage threading barrier for object supplier chain.
*/
class CInformationProviderCacheComp:
			public icomp::CComponentBase,
			virtual public istd::IInformationProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CInformationProviderCacheComp);
		I_REGISTER_INTERFACE(IInformationProvider);
	I_END_COMPONENT;

	CInformationProviderCacheComp();

	// reimplemented (iinsp::IInformationProvider)
	virtual QDateTime GetInformationTimeStamp() const;
	virtual InformationCategory GetInformationCategory() const;
	virtual int GetInformationId() const;
	virtual QString GetInformationDescription() const;
	virtual QString GetInformationSource() const;
	virtual int GetInformationFlags() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	QDateTime m_timeStamp;
	InformationCategory m_category;
	int m_id;
	QString m_description;
	QString m_source;
	int m_flags;
};


} // namespace iinsp


#endif // !iinsp_CInformationProviderCacheComp_included


