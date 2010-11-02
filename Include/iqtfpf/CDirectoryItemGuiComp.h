#ifndef iqtfpf_CDirectoryItemGuiComp_included
#define iqtfpf_CDirectoryItemGuiComp_included


// ACF includes
#include "iprm/IParamsManager.h"
#include "iprm/IFileNameParam.h"
#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ifpf/IHotfolderStatistics.h"

#include "iqtfpf/Generated/ui_CDirectoryItemGuiComp.h"


namespace iqtfpf
{


class CDirectoryItemGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CDirectoryItemGuiComp, ifpf::IHotfolderStatistics>,
			virtual public iprm::ISelectionParam
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CDirectoryItemGuiComp,
				ifpf::IHotfolderStatistics> BaseClass;

	I_BEGIN_COMPONENT(CDirectoryItemGuiComp)
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
		I_ASSIGN(m_inputDirectoriesParamsManagerCompPtr, "InputDirectoriesManager", "Parameter manager for the input directories", true, "InputDirectoriesManager");
	I_END_COMPONENT;

	CDirectoryItemGuiComp();

	void SetDirectoryPath(const QString& directoryPath);

	// reimplemented (iprm::ISelectionParam)
	virtual const iprm::ISelectionConstraints* GetConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual iprm::ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

private:
	void UpdateProgressBar(const ifpf::IHotfolderStatistics& statistics);

	QString GenerateStyleSheet(double successed, double errors, double aborted) const;

private:
	I_REF(iprm::IParamsManager, m_inputDirectoriesParamsManagerCompPtr);

	QString m_directoryPath;

	int m_setIndex;
};


} // namespace iqtfpf


#endif // !iqtfpf_CDirectoryItemGuiComp_included


