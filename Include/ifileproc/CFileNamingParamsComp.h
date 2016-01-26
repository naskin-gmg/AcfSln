#ifndef ifileproc_CFileNamingParamsComp_included
#define ifileproc_CFileNamingParamsComp_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "ifileproc/CFileNamingParams.h"


namespace ifileproc
{


class CFileNamingParamsComp:
			public icomp::CComponentBase,
			public ifileproc::CFileNamingParams
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef ifileproc::CFileNamingParams BaseClass2;

	I_BEGIN_COMPONENT(ifileproc::CFileNamingParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ifileproc::IFileNamingParams);
		I_ASSIGN(m_prefixAttrPtr, "Prefix", "Prefix attached to the output file name", true, "");
		I_ASSIGN(m_suffixAttrPtr, "Suffix", "Suffix attached to the output file name", true, "");
		I_ASSIGN(m_overwriteStrategyAttrPtr, "OverwriteStrategy", "Strategy for overwriting of existing files.\n0 - Overwrite existing file\n1 - Add auto generated numbered suffix to the output file name", true, 0);
		I_ASSIGN_MULTI_0(m_patternsToRemoveAttrPtr, "PatternsToRemove", "List of patterns that will be removed from the output file name", false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(QString, m_prefixAttrPtr);
	I_ATTR(QString, m_suffixAttrPtr);
	I_ATTR(int, m_overwriteStrategyAttrPtr);
	I_MULTIATTR(QString, m_patternsToRemoveAttrPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CFileNamingParamsComp_included


