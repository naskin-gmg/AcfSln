#ifndef iqtmeas_CDataSequenceSupplierResultsViewComp_included
#define iqtmeas_CDataSequenceSupplierResultsViewComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#else
#include <QtGui/QVBoxLayout>
#endif

// ACF includes
#include "imod/TSingleModelObserverBase.h"
#include "iinsp/ISupplier.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/TGuiComponentBase.h"

// ACF-Solutions includes
#include "imeas/IDataSequenceProvider.h"


namespace iqtmeas
{


class CDataSequenceSupplierResultsViewComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QWidget>,
						imod::TSingleModelObserverBase<imeas::IDataSequenceProvider> >
{
public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QWidget>,
				imod::TSingleModelObserverBase<imeas::IDataSequenceProvider> > BaseClass;

	I_BEGIN_COMPONENT(CDataSequenceSupplierResultsViewComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_resultsGuiCompPtr, "ResultsGui", "GUI for the supplier's results", true, "ResultsGui");
		I_ASSIGN_TO(m_resultsObserverCompPtr, m_resultsGuiCompPtr, true);
	I_END_COMPONENT;


protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_resultsGuiCompPtr);
	I_REF(imod::IObserver, m_resultsObserverCompPtr);
};


} // namespace iqtmeas


#endif // !iqtmeas_CDataSequenceSupplierResultsViewComp_included


