#ifndef iqtfpf_CHotfolderWorkflowScenographerComp_included
#define iqtfpf_CHotfolderWorkflowScenographerComp_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"


#include "i2d/CVector2d.h"

#include "iqt2d/TScenographerCompBase.h"
#include "iqt2d/CSceneProviderGuiComp.h"


// AcfSln includes
#include "ifpf/IHotfolderWorkflow.h"


namespace iqtfpf
{


/**
	Hotfolder workflow user interface.
*/
class CHotfolderWorkflowScenographerComp:
			public QObject,
			public iqt2d::TScenographerCompBase<
						imod::TSingleModelObserverBase<ifpf::IHotfolderWorkflow> >
{
	Q_OBJECT

public:
	typedef iqt2d::TScenographerCompBase<
				imod::TSingleModelObserverBase<ifpf::IHotfolderWorkflow> > BaseClass;

	I_BEGIN_COMPONENT(CHotfolderWorkflowScenographerComp);
	I_END_COMPONENT;

	CHotfolderWorkflowScenographerComp();

protected:
	bool TryCreateHotfolder(const istd::CString& hofolderId, const i2d::CVector2d& position);
	QGraphicsItem* AddShapeToScene(istd::IPolymorphic* elementPtr) const;

	// reimplemented (iqt2d::TScenographerCompBase)
	virtual bool OnDropObject(const QMimeData& mimeData, QGraphicsSceneDragDropEvent* eventPtr);
	virtual void UpdateScene(int updateFlags);
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderWorkflowScenographerComp_included


