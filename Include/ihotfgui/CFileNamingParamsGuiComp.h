#ifndef ihotfgui_CFileNamingParamsGuiComp_included
#define ihotfgui_CFileNamingParamsGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"

// ACF-Solutions includes
#include "ifileproc/IFileNamingParams.h"
#include "GeneratedFiles/ihotfgui/ui_CFileNamingParamsGuiComp.h"


namespace ihotfgui
{


class CFileNamingParamsGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileNamingParamsGuiComp,
						ifileproc::IFileNamingParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileNamingParamsGuiComp,
				ifileproc::IFileNamingParams> BaseClass;

	I_BEGIN_COMPONENT(CFileNamingParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void OnModelChanged();
};


} // namespace ihotfgui


#endif // !ihotfgui_CFileNamingParamsGuiComp_included


