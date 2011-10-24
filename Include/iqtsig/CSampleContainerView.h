#ifndef iqtsig_CSampleContainerView_included
#define iqtsig_CSampleContainerView_included


#include <QWidget>

#include "istd/CRange.h"

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
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* event);

private:
	istd::CRange m_displayedRange;
};


} // namespace iqtsig


#endif // !iqtsig_CSampleContainerView_included


