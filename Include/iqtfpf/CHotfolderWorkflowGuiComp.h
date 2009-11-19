#ifndef iqtfpf_CHotfolderWorkflowGuiComp_included
#define iqtfpf_CHotfolderWorkflowGuiComp_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/TGuiComponentBase.h"

#include "iqt2d/CSceneProviderGuiComp.h"


// AcfSln includes
#include "ifpf/IHotfolderWorkflow.h"


namespace iqtfpf
{


/**
	Hotfolder workflow user interface.
*/
class CHotfolderWorkflowGuiComp:
			public ibase::TLoggerCompWrap<
						iqtgui::TGuiObserverWrap<
									iqt2d::CSceneProviderGuiComp, 
									imod::TSingleModelObserverBase<ifpf::IHotfolderWorkflow> > >
{
	Q_OBJECT
public:
	typedef ibase::TLoggerCompWrap<
				iqtgui::TGuiObserverWrap<
							iqt2d::CSceneProviderGuiComp, 
							imod::TSingleModelObserverBase<ifpf::IHotfolderWorkflow> > > BaseClass;

	I_BEGIN_COMPONENT(CHotfolderWorkflowGuiComp);
		I_ASSIGN(m_hotfolderFactCompPtr, "HotfolderFactory", "Factory for creation of a hotfolder", true, "HotfolderFactory");
	I_END_COMPONENT();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

private:
	I_FACT(ifpf::IHotfolderParams, m_hotfolderFactCompPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderWorkflowGuiComp_included


