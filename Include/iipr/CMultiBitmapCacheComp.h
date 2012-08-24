#ifndef iipr_CMultiBitmapCacheComp_included
#define iipr_CMultiBitmapCacheComp_included


// ACF includes
#include "istd/TPointerVector.h"
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iipr/IMultiBitmapProvider.h"


namespace iipr
{


/**
	Multi bitmap provider returning always stored bitmaps.
	Stored bitmaps can be changed only copying their from another multi bitmap provider using CopyFrom method.
	This object will be used to manage threading barrier for object supplier chain.
*/
class CMultiBitmapCacheComp:
			public icomp::CComponentBase,
			virtual public IMultiBitmapProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMultiBitmapCacheComp);
		I_REGISTER_INTERFACE(IMultiBitmapProvider);
	I_END_COMPONENT;

	CMultiBitmapCacheComp();

	// reimplemented (iipr::IMultiBitmapProvider)
	virtual const iprm::ISelectionConstraints* GetBitmapSelectionContraints() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;
	virtual const i2d::ITransformation2d* GetLogTransform(int bitmapIndex) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

private:
	istd::TPointerVector<iimg::IBitmap> m_bitmapsPtr;
	istd::TPointerVector<i2d::ITransformation2d> m_transformsPtr;
};


} // namespace iipr


#endif // !iipr_CMultiBitmapCacheComp_included


