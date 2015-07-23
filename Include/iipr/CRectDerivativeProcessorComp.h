#ifndef iipr_CRectDerivativeProcessorComp_included
#define iipr_CRectDerivativeProcessorComp_included


#include <QtCore/QString>

#include "icomp/CComponentBase.h"

#include "iipr/CRectDerivativeProcessor.h"


namespace iipr
{


/**
	Component implementation of derivative processor using rectangular filter kernel.
*/
class CRectDerivativeProcessorComp: public icomp::CComponentBase, public CRectDerivativeProcessor
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CRectDerivativeProcessor BaseClass2;

	I_BEGIN_COMPONENT(CRectDerivativeProcessorComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_INTERFACE(INumericConstraints);
		I_ASSIGN(m_filterParamsIdAttrPtr, "FilterParamsId", "ID of filter parameters in parameter set", true, "FilterParams");
		I_ASSIGN(m_doublePrecisionAttrPtr, "DoublePrecision", "If set double precision will be used during the calculation", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(QByteArray, m_filterParamsIdAttrPtr);
	I_ATTR(bool, m_doublePrecisionAttrPtr);
};


} // namespace iipr


#endif // !iipr_CRectDerivativeProcessorComp_included


