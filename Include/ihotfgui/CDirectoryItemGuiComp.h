#ifndef ihotfgui_CDirectoryItemGuiComp_included
#define ihotfgui_CDirectoryItemGuiComp_included


// ACF includes
#include "iprm/IParamsManager.h"
#include "ifile/IFileNameParam.h"
#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ihotf/IHotfolderStatistics.h"

#include "ihotfgui/Generated/ui_CDirectoryItemGuiComp.h"


namespace ihotfgui
{


class CDirectoryItemGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CDirectoryItemGuiComp, ihotf::IHotfolderStatistics>,
			virtual public iprm::ISelectionParam
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CDirectoryItemGuiComp,
				ihotf::IHotfolderStatistics> BaseClass;

	I_BEGIN_COMPONENT(CDirectoryItemGuiComp);
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
		I_ASSIGN(m_inputDirectoriesParamsManagerCompPtr, "InputDirectoriesManager", "Parameter manager for the input directories", true, "InputDirectoriesManager");
		I_ASSIGN(m_directoryPathIdAttrPtr, "DirParamId", "Parameter ID of the directory path", true, "DirectoryPath");
	I_END_COMPONENT;

	CDirectoryItemGuiComp();

	// reimplemented (iprm::ISelectionParam)
	virtual const iprm::ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual iprm::ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

private:
	void UpdateProgressBar(const ihotf::IHotfolderStatistics& statistics);

	QString GenerateStyleSheet(double successed, double errors, double aborted) const;

private:
	I_REF(iprm::IParamsManager, m_inputDirectoriesParamsManagerCompPtr);
	I_ATTR(QByteArray, m_directoryPathIdAttrPtr);

	QString m_directoryPath;
	QString m_hotfolderInputName;

	int m_setIndex;
};


} // namespace ihotfgui


#endif // !ihotfgui_CDirectoryItemGuiComp_included


