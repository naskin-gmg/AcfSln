#include "ihotf/CVisualHotfolderWorkflowComp.h"


// Qt includes
#include <QtCore/QStringList>


// ACF includes
#include "istd/TChangeNotifier.h"


// public methods


namespace ihotf
{


bool CVisualHotfolderWorkflowComp::SerializeLayout(iser::IArchive& archive)
{
	static iser::CArchiveTag positionMapTag("PositionMap", "Map of component name to its positions");
	static iser::CArchiveTag elementTag("Element", "Map element");

	bool retVal = true;

	QStringList ids = this->GetHotfolderIds();
	int positionsCount = ids.size();

	retVal = retVal && archive.BeginMultiTag(positionMapTag, elementTag, positionsCount);

	if (!retVal){
		return false;
	}

	if (archive.IsStoring()){
		for (QStringList::const_iterator iter = ids.begin(); iter != ids.end(); ++iter){
			QString elementId = *iter;

			i2d::CVector2d position(0, 0);
			const CVisualHotfolderWorkflowItem* elementPtr = dynamic_cast<const CVisualHotfolderWorkflowItem*>(GetHotfolder(elementId));
			if (elementPtr != NULL){
				position = elementPtr->GetCenter();
			}

			retVal = retVal && archive.BeginTag(elementTag);

			retVal = retVal && SerializeItemPosition(archive, elementId, position);
				
			retVal = retVal && archive.EndTag(elementTag);
		}
	}
	else{
		istd::CChangeNotifier notifier(this);

		for (int i = 0; i < positionsCount; ++i){
			retVal = retVal && archive.BeginTag(elementTag);
			
			QString elementId;
			i2d::CVector2d position;

			retVal = retVal && SerializeItemPosition(archive, elementId, position);
			if (!retVal){
				return false;
			}

			CVisualHotfolderWorkflowItem* elementPtr = dynamic_cast<CVisualHotfolderWorkflowItem*>(GetHotfolder(elementId));
			if (elementPtr != NULL){
				elementPtr->MoveCenterTo(position);
			}

			retVal = retVal && archive.EndTag(elementTag);
		}
	
	}

	retVal = retVal && archive.EndTag(positionMapTag);

	return retVal;
}


bool CVisualHotfolderWorkflowComp::SerializeWorkflow(iser::IArchive& archive)
{
	return BaseClass::Serialize(archive);
}


// reimplemented (iser::ISerializable)

bool CVisualHotfolderWorkflowComp::Serialize(iser::IArchive& archive)
{
	return SerializeWorkflow(archive) && SerializeLayout(archive);
}


// protected methods

bool CVisualHotfolderWorkflowComp::SerializeItemPosition(iser::IArchive& archive, QString& hotfolderName, i2d::CVector2d& position)
{
	static iser::CArchiveTag nameTag("HotfolderName", "Name of the hotfolder");
	static iser::CArchiveTag positionXTag("X", "X position of component");
	static iser::CArchiveTag positionYTag("Y", "Y position of component");
	
	bool retVal = archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(hotfolderName);
	retVal = retVal && archive.EndTag(nameTag);

	retVal = retVal && archive.BeginTag(positionXTag);
	retVal = retVal && archive.Process(position[0]);
	retVal = retVal && archive.EndTag(positionXTag);

	retVal = retVal && archive.BeginTag(positionXTag);
	retVal = retVal && archive.Process(position[1]);
	retVal = retVal && archive.EndTag(positionXTag);

	return retVal;
}


// reimplemented (ihotf::CHotfolderWorkflowComp)

ihotf::IHotfolderWorkflowItem* CVisualHotfolderWorkflowComp::CreateWorkflowItem(const QString& hotfolderId) const
{
	Element* elementPtr = new Element();

	elementPtr->Initialize(hotfolderId, this);

	return elementPtr;
}

	
} // namespace ihotf


