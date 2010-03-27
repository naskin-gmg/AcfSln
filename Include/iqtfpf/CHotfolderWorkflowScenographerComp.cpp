#include "iqtfpf/CHotfolderWorkflowScenographerComp.h"


// Qt includes
#include <QInputDialog>
#include <QMessageBox>


// ACF includes
#include "iser/CMemoryReadArchive.h"


namespace iqtfpf
{


// public methods

CHotfolderWorkflowScenographerComp::CHotfolderWorkflowScenographerComp()
{
	SetAcceptedMimeTypes(QStringList() << "hotfolder");
}


// protected methods

bool CHotfolderWorkflowScenographerComp::TryCreateHotfolder(const istd::CString& hotfolderId, const i2d::CVector2d& /*position*/)
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
			iprm::IParamsSet* hotfolderPtr = workflowPtr->AddHotfolder(hotfolderName.toStdString(), hotfolderId);
			if (hotfolderPtr != NULL){
				return true;
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

	i2d::CVector2d position(0, 0);
	if (eventPtr != NULL){
		position = iqt::GetCVector2d(eventPtr->scenePos());
	}

	return TryCreateHotfolder(hotfolderId, position);

}


void CHotfolderWorkflowScenographerComp::UpdateScene(int /*updateFlags*/)
{

}


} // namespace iqtfpf


