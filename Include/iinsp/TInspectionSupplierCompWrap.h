#ifndef iinsp_TInspectionSupplierCompWrap_h
#define iinsp_TInspectionSupplierCompWrap_h


// ACF includes
#include "iinsp/TSupplierCompWrap.h"


namespace iinsp
{


template <class Product>
class TInspectionSupplierCompWrap: public TSupplierCompWrap<Product>
{
public:
	I_BEGIN_BASE_COMPONENT(TInspectionSupplierCompWrap);
		I_ASSIGN(m_errorGroupIdAttrPtr, "ErrorGroupId", "ID of the error group (i.e. Tab, End etc)", true, "");
		I_ASSIGN(m_errorClassIdAttrPtr, "ErrorClassId", "ID of the error class (i.e. Position, Color etc)", true, "");
	I_END_COMPONENT;

protected:
	I_ATTR(QString, m_errorGroupIdAttrPtr);
	I_ATTR(QString, m_errorClassIdAttrPtr);

	void AddGroupError() const
	{
		QString errorGroup(*m_errorGroupIdAttrPtr);
		QString errorClass(*m_errorClassIdAttrPtr);

		if (!errorClass.isEmpty()){
			ilog::CMessage* errorClassMessagePtr = new ilog::CMessage(
				istd::IInformationProvider::IC_ERROR,
				20001, //qstd::MI_INSPECTION_GROUP,
				QString(),
				errorGroup.isEmpty() ? errorClass: QString("%1.%2").arg(errorGroup, errorClass));	// i.e. Tab.Search...

			AddMessage(errorClassMessagePtr);
		}

		if (!errorGroup.isEmpty()){
			ilog::CMessage* errorGroupMessagePtr = new ilog::CMessage(
				istd::IInformationProvider::IC_ERROR,
				20001, //qstd::MI_INSPECTION_GROUP,
				QString(),
				errorGroup);	// i.e. Tab, Panel...

			AddMessage(errorGroupMessagePtr);
		}
	}
};


}


#endif //!iinsp_TInspectionSupplierCompWrap_h