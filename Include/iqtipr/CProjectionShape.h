#ifndef iqtipr_CProjectionShape_included
#define iqtipr_CProjectionShape_included


// Qt includes
#include <QGraphicsPathItem>

// ACF includes
#include "imod/CMultiModelObserverBase.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"

#include "iqtipr/iqtipr.h"


namespace iqtipr
{


class CProjectionShape: public QGraphicsPathItem, public imod::CMultiModelObserverBase
{
public:
	typedef imod::CMultiModelObserverBase BaseClass;

	CProjectionShape();

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	i2d::CLine2d m_projectionLine;
};


} // namespace iqtipr


#endif // !iqtipr_CProjectionShape_included


