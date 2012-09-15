#ifndef iipr_CTubeProjectionLinesProviderComp_included
#define iipr_CTubeProjectionLinesProviderComp_included


// ACF-Solutions includes
#include "iipr/CMultiLineSupplierCompBase.h"


namespace iipr
{


class CTubeProjectionLinesProviderComp: public iipr::CMultiLineSupplierCompBase
{
public:
	typedef iipr::CMultiLineSupplierCompBase BaseClass;

	I_BEGIN_COMPONENT(CTubeProjectionLinesProviderComp);
		I_ASSIGN(m_tubeParamsIdAttrPtr, "TubeParamsId", "ID of the tube parameter in the parameter set", true, "TubeParamsId");
		I_ASSIGN(m_linesCountParamsIdAttrPtr, "LinesCountParamsId", "ID of the line count parameter in the parameter set", true, "LinesCountParamsId");
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::CMultiLineSupplierCompBase)
	virtual int ProduceObject(ProductType& result) const;

private:
	I_ATTR(QByteArray, m_tubeParamsIdAttrPtr);
	I_ATTR(QByteArray, m_linesCountParamsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CTubeProjectionLinesProviderComp_included


