// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icmpstr_CConvertComponentCommandComp_included
#define icmpstr_CConvertComponentCommandComp_included


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <icomp/IMetaInfoManager.h>
#include <icomp/CComponentBase.h>
#include <ibase/ICommandsProvider.h>
#include <iqtgui/CHierarchicalCommand.h>
#include <icmpstr/IElementSelectionInfo.h>

#include <GeneratedFiles/icmpstr/ui_CConvertComponentCommandComp.h>


namespace icmpstr
{


class CConvertComponentCommandComp:
			public QDialog,
			public Ui::CConvertComponentCommandComp,
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<IElementSelectionInfo>,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<IElementSelectionInfo> BaseClass2;
	typedef QDialog BaseClass3;

	I_BEGIN_COMPONENT(CConvertComponentCommandComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Access to manager of components meta information", true, "MetaInfoManager");
	I_END_COMPONENT;

public:
	CConvertComponentCommandComp();

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const override;

protected:
	enum GroupId
	{
		GI_COMPONENT = 0x5430
	};

	/**
		Insert supported and used features to list of component filter.
	*/
	void InsertElementToFilters(
				const icomp::IRegistry& registry,
				const QByteArray& elementName,
				const icomp::IRegistry::ElementInfo& elementInfo);

	void CreatePackagesList();
	void CreateComponentsList();
	void CalcFilteredComponents();
	void DoRetranslate();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_InterfacesPolicyCB_currentIndexChanged(int index);
	void on_AttributesPolicyCB_currentIndexChanged(int index);
	void on_SubcomponentsPolicyCB_currentIndexChanged(int index);
	void on_FilterEdit_textChanged(const QString& text);
	void on_PackageNameCB_currentIndexChanged(int index);
	void on_ComponentNameCB_currentIndexChanged(int index);
	void OnPromoteCommand();

private:
	enum FilterType
	{
		FT_IGNORED,
		FT_USED,
		FT_SUPPORTED
	};

	struct Filter
	{
		icomp::IElementStaticInfo::Ids elements;
		icomp::IElementStaticInfo::Ids interfaces;
		iattr::IAttributesProvider::AttributeIds attributes;
		icomp::IElementStaticInfo::Ids subcomponents;
	};

	I_REF(icomp::IMetaInfoManager, m_metaInfoManagerCompPtr);

	Filter m_supportedFilter;
	Filter m_usedFilter;

	icomp::IMetaInfoManager::ComponentAddresses m_filteredComponents;

	iqtgui::CHierarchicalCommand m_rootMenuCommand;
	iqtgui::CHierarchicalCommand m_registryMenu;
	iqtgui::CHierarchicalCommand m_convertComponentCommand;
};


} // namespace icmpstr


#endif // !icmpstr_CConvertComponentCommandComp_included


