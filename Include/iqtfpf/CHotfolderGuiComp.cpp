#include "iqtfpf/CHotfolderGuiComp.h"


// Qt includes
#include <QHeaderView>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"

#include "iqtgui/CItemDelegate.h"


namespace iqtfpf
{


// reimplemented (imod::IModelEditor)

void CHotfolderGuiComp::UpdateModel() const
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && !IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CHotfolderGuiComp*>(this));
	}
}


void CHotfolderGuiComp::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(this, true);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CHotfolderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


} // namespace iqtfpf


