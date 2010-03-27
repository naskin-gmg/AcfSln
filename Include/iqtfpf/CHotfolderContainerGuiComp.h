#ifndef iqtfpf_CHotfolderContainerGuiComp_included
#define iqtfpf_CHotfolderContainerGuiComp_included


// Qt includes
#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>
#include <QDir>


// ACF includes
#include "istd/TDelPtr.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "Generated/ui_CHotfolderContainerGuiComp.h"


namespace iqtfpf
{


class CHotfolderContainerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CHotfolderContainerGuiComp>
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CHotfolderContainerGuiComp> BaseClass;
	
	I_BEGIN_COMPONENT(CHotfolderContainerGuiComp);
		I_ASSIGN_MULTI_0(m_hotoflderFactoryIdsAttrPtr, "HotfolderFactoryIds", "List of factory IDs for hotfolder creation", true);
	I_END_COMPONENT;

protected:
	/**
		Create the drag preview for the component.
	*/
	QPixmap CreateComponentDragPixmap(const istd::CString& hotfolderId) const;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_MULTIATTR(istd::CString, m_hotoflderFactoryIdsAttrPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderContainerGuiComp_included

