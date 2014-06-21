#ifndef iqtsig_CSampleContainerView_included
#define iqtsig_CSampleContainerView_included


#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include "istd/TRange.h"
#include "imod/TSingleModelObserverBase.h"
#include "imeas/IDataSequence.h"


namespace iqtsig
{


class CSampleContainerView: public QWidget, public imod::TSingleModelObserverBase<imeas::IDataSequence>
{
public:
	typedef QWidget BaseClass;
	typedef imod::TSingleModelObserverBase<imeas::IDataSequence> BaseClass2;

	CSampleContainerView(QWidget* parentPtr = NULL);

	void SetDisplayedRange(const istd::CRange& range);

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* event);

private:
	istd::CRange m_displayedRange;
};


} // namespace iqtsig


#endif // !iqtsig_CSampleContainerView_included


