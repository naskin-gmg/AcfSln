#ifndef icam_CMemoryAcquisitionComp_included
#define icam_CMemoryAcquisitionComp_included


// ACF includes
#include "iser/IFileLoader.h"
#include "icomp/CComponentBase.h"
#include "iprm/IFileNameParam.h"
#include "iimg/CGeneralBitmap.h"

#include "iproc/TSyncProcessorWrap.h"

#include "iproc/IBitmapAcquisition.h"

#include "icam/icam.h"


namespace icam
{


/**
	Bitmap loader component implementing interfaces \c iser::IFileLoader and \c iproc::IBitmapAcquisition.
*/
class CMemoryAcquisitionComp:
			public icomp::CComponentBase,
			public iimg::CGeneralBitmap,
			virtual public iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iimg::CGeneralBitmap BaseClass2;

	I_BEGIN_COMPONENT(CMemoryAcquisitionComp);
		I_REGISTER_INTERFACE(iproc::IBitmapAcquisition);
		I_REGISTER_INTERFACE(iimg::IBitmap);
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;
};


} // namespace icam


#endif // !icam_CMemoryAcquisitionComp_included


