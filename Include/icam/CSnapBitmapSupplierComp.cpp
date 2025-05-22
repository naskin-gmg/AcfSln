#include <icam/CSnapBitmapSupplierComp.h>


namespace icam
{


// protected methods

iimg::IBitmapUniquePtr CSnapBitmapSupplierComp::CreateBitmap() const
{
	if (m_bitmapCompFact.IsValid()){
		return m_bitmapCompFact.CreateInstance();
	}

	return NULL;
}


} // namespace icam


