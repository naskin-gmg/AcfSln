#pragma once



// ACF-Solutions includes
#include <icam/CCameraSnapSupplierCompBase.h>


namespace icam
{


/**
	Factory-based Snap Bitmap Supplier (not thread-safe). 
*/
class CSnapBitmapSupplierComp: public CCameraSnapSupplierCompBase
{
public:
	typedef CCameraSnapSupplierCompBase BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierComp);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
	I_END_COMPONENT;

protected:
	// reimplemented (CSnapBitmapSupplierCompBase)
	virtual iimg::IBitmapUniquePtr CreateBitmap() const override;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);
};


} // namespace icam


