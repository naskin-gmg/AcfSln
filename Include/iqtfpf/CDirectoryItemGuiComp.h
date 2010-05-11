#ifndef iqtfpf_CDirectoryItemGuiComp_included
#define iqtfpf_CDirectoryItemGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ifpf/IHotfolderStatistics.h"

#include "iqtfpf/Generated/ui_CDirectoryItemGuiComp.h"


namespace iqtfpf
{


class CDirectoryItemGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CDirectoryItemGuiComp, ifpf::IHotfolderStatistics>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CDirectoryItemGuiComp,
				ifpf::IHotfolderStatistics> BaseClass;

	I_BEGIN_COMPONENT(CDirectoryItemGuiComp)
	I_END_COMPONENT;

	void SetDirectoryPath(const QString& directoryPath);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

private:
	void UpdateProgressBar(const ifpf::IHotfolderStatistics& statistics);

	QString GenerateStyleSheet(double successed, double errors, double aborted) const;

private:
	QString m_directoryPath;
};


} // namespace iqtfpf


#endif // !iqtfpf_CDirectoryItemGuiComp_included


