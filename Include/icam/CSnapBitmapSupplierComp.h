#pragma once



// ACF-Solutions includes
#include <icam/CSnapBitmapSupplierCompBase.h>


namespace icam
{


/**
	Factory-based Snap Bitmap Supplier (not thread-safe). 
*/
class CSnapBitmapSupplierComp: public CSnapBitmapSupplierCompBase
{
public:
	typedef CSnapBitmapSupplierCompBase BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierComp);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
	I_END_COMPONENT;

protected:
	// reimplemented (CSnapBitmapSupplierCompBase)
	virtual iimg::IBitmap* CreateBitmap() const;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);
};


} // namespace icam


