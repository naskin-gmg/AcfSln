#ifndef ifpf_CFileNamingParamsComp_included
#define ifpf_CFileNamingParamsComp_included


// Qt includes
#include <QStringList>

// ACF includes
#include "icomp/CComponentBase.h"

// AcfSln includes
#include "ifpf/IFileNamingParams.h"


namespace ifpf
{


class CFileNamingParamsComp:
			public icomp::CComponentBase,
			virtual public ifpf::IFileNamingParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	CFileNamingParamsComp();

	I_BEGIN_COMPONENT(ifpf::CFileNamingParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ifpf::IFileNamingParams);
		I_ASSIGN(m_prefixAttrPtr, "Prefix", "Prefix attached to the output file name", true, "");
		I_ASSIGN(m_suffixAttrPtr, "Suffix", "Suffix attached to the output file name", true, "");
		I_ASSIGN(m_overwriteStrategyAttrPtr, "OverwriteStrategy", "Strategy for overwriting of existing files.\n0 - Overwrite existing file\n1 - Add auto generated numbered suffix to the output file name", true, 0);
		I_ASSIGN_MULTI_0(m_patternsToRemoveAttrPtr, "PatternsToRemove", "List of patterns that will be removed from the output file name", false);
	I_END_COMPONENT;

	// reimplemented (ifpf::IFileNamingParams)
	virtual OverwriteStrategy GetOverwriteStrategy() const;
	virtual void SetOverwriteStrategy(OverwriteStrategy overwriteStrategy);
	virtual QString GetPrefix() const;
	virtual void SetPrefix(const QString& prefix);
	virtual QString GetSuffix() const;
	virtual void SetSuffix(const QString& suffix);
	virtual QStringList GetPatternsToRemove() const;
	virtual void SetPatternsToRemove(const QStringList& patternsToRemove);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	OverwriteStrategy m_overwriteStrategy;
	QString m_suffix;
	QString m_prefix;
	QStringList m_patternsToRemove;

	I_ATTR(QString, m_prefixAttrPtr);
	I_ATTR(QString, m_suffixAttrPtr);
	I_ATTR(int, m_overwriteStrategyAttrPtr);
	I_MULTIATTR(QString, m_patternsToRemoveAttrPtr);
};


} // namespace ifpf


#endif // !ifpf_CFileNamingParamsComp_included


