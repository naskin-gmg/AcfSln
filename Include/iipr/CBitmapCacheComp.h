#ifndef iipr_CBitmapCacheComp_included
#define iipr_CBitmapCacheComp_included


// ACF includes
#include "istd/TDelPtr.h"
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iipr/IBitmapProvider.h"


namespace iipr
{


/**
	Bitmap provider returning always stored bitmap.
	Stored bitmap can be changed only copying the bitmap from another bitmap provider using CopyFrom method.
	This object will be used to manage threading barrier for object supplier chain.
*/
class CBitmapCacheComp:
			public icomp::CComponentBase,
			virtual public IBitmapProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CBitmapCacheComp);
		I_REGISTER_INTERFACE(IBitmapProvider);
	I_END_COMPONENT;

	// reimplemented (iipr::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;
	virtual const i2d::ITransformation2d* GetLogTransform() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

private:
	istd::TDelPtr<iimg::IBitmap> m_bitmapPtr;
	istd::TDelPtr<i2d::ITransformation2d> m_transformPtr;
};


} // namespace iipr


#endif // !iipr_CBitmapCacheComp_included


