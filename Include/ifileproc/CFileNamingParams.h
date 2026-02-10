// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ifileproc_CFileNamingParams_included
#define ifileproc_CFileNamingParams_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <icomp/CComponentBase.h>

// AcfSln includes
#include <ifileproc/IFileNamingParams.h>


namespace ifileproc
{


class CFileNamingParams: virtual public ifileproc::IFileNamingParams
{
public:
	CFileNamingParams();

	// reimplemented (ifileproc::IFileNamingParams)
	virtual OverwriteStrategy GetOverwriteStrategy() const override;
	virtual void SetOverwriteStrategy(OverwriteStrategy overwriteStrategy) override;
	virtual QString GetPrefix() const override;
	virtual void SetPrefix(const QString& prefix) override;
	virtual QString GetSuffix() const override;
	virtual void SetSuffix(const QString& suffix) override;
	virtual QStringList GetPatternsToRemove() const override;
	virtual void SetPatternsToRemove(const QStringList& patternsToRemove) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const istd::IChangeable& object) const override;
	virtual istd::IChangeableUniquePtr CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

protected:
	OverwriteStrategy m_overwriteStrategy;
	QString m_suffix;
	QString m_prefix;
	QStringList m_patternsToRemove;
};


} // namespace ifileproc


#endif // !ifileproc_CFileNamingParams_included


