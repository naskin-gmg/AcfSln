#include "iqtfpf/CHotfolderWorkflowScenographerComp.h"


// Qt includes
#include <QInputDialog>
#include <QMessageBox>


// ACF includes
#include "iser/CMemoryReadArchive.h"


// AcfSln incldues
#include "ifpf/CVisualHotfolderWorkflowItem.h"

#include "iqtfpf/CHotfolderShape.h"


namespace iqtfpf
{


// public methods

CHotfolderWorkflowScenographerComp::CHotfolderWorkflowScenographerComp()
{
	SetAcceptedMimeTypes(QStringList() << "hotfolder");
}


// protected methods

bool CHotfolderWorkflowScenographerComp::TryCreateHotfolder(const istd::CString& hotfolderId, const i2d::CVector2d& position)
{
	bool retVal = false;
	
	QString hotfolderName = QInputDialog::getText(
				NULL, 
				tr("Workflow designer"), 
				tr("Hotfolder name"), 
				QLineEdit::Normal, 
				iqt::GetQString(hotfolderId),
				&retVal);
	
	if (retVal && !hotfolderName.isEmpty()){
		istd::TChangeNotifier<ifpf::IHotfolderWorkflow> workflowPtr(GetObjectPtr());
		if (workflowPtr.IsValid()){
			ifpf::IHotfolderWorkflowItem* workflowItemPtr = workflowPtr->AddHotfolder(iqt::GetCString(hotfolderName), hotfolderId);
			if (workflowItemPtr != NULL){
				return true;
			}

			ifpf::CVisualHotfolderWorkflowItem* visualItemPtr = dynamic_cast<ifpf::CVisualHotfolderWorkflowItem*>(workflowItemPtr);
			if (visualItemPtr != NULL){
				visualItemPtr->MoveTo(position);
			}
		}

		QMessageBox::critical(NULL, tr("Error"), tr("Hotfolder could not be added")); 
	}

	return false;
}


// reimplemented (iqt2d::TScenographerCompBase)

bool CHotfolderWorkflowScenographerComp::OnDropObject(const QMimeData& mimeData, QGraphicsSceneDragDropEvent* eventPtr)
{
	QByteArray byteData = mimeData.data("hotfolder");
	iser::CMemoryReadArchive archive(byteData.constData(), byteData.size());

	istd::CString hotfolderId;

	bool retVal = archive.Process(hotfolderId);

	i2d::CVector2d position(0, 0);
	if (retVal && eventPtr != NULL){
		position = iqt::GetCVector2d(eventPtr->scenePos());
	}

	return TryCreateHotfolder(hotfolderId, position);

}


void CHotfolderWorkflowScenographerComp::UpdateScene(int /*updateFlags*/)
{
	QList<QGraphicsItem*> items = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, items){
		m_scenePtr->removeItem(itemPtr);
	}

	ifpf::IHotfolderWorkflow* workflowPtr = GetObjectPtr();
	if (workflowPtr != NULL){
		istd::CStringList hotfolderList = workflowPtr->GetHotfolderList();
		for (int hotfolderIndex = 0; hotfolderIndex < int(hotfolderList.size()); hotfolderIndex++){
			ifpf::IHotfolderWorkflowItem* workflowItemPtr = workflowPtr->GetHotfolder(hotfolderList[hotfolderIndex]);
				if (workflowItemPtr != NULL){
					AddShapeToScene(workflowItemPtr);
				}
		}
	}

}


// protected methods

QGraphicsItem* CHotfolderWorkflowScenographerComp::AddShapeToScene(istd::IPolymorphic* elementPtr) const
{
	if (m_scenePtr == NULL){
		return NULL;
	}

	imod::IModel* elementModelPtr = dynamic_cast<imod::IModel*>(elementPtr);
	if (elementModelPtr != NULL){
		ifpf::CVisualHotfolderWorkflowItem* workflowItemPtr = dynamic_cast<ifpf::CVisualHotfolderWorkflowItem*>(elementPtr);
		if (workflowItemPtr != NULL){
			CHotfolderShape* shapePtr = new CHotfolderShape(m_sceneProviderCompPtr.GetPtr());

			elementModelPtr->AttachObserver(shapePtr);

			m_scenePtr->addItem(shapePtr);

			return shapePtr;
		}
	}

	return NULL;
}


} // namespace iqtfpf


