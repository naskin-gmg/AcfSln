#ifndef iipr_CPositionFromImageSupplierComp_included
#define iipr_CPositionFromImageSupplierComp_included


// ACF includes
#include "i2d/CVector2d.h"
#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"
#include "iproc/IValueProvider.h"

#include "iipr/IBitmapProvider.h"


namespace iipr
{


/**
	Implementation of value supplier returning position on the image.
	To calculate this position is some processor used.
	This processor must accept image object as input and produce some set of features containing position into feature consumer.
	This supplier takes the feature with the higher weight value and output it as found position value.
*/
class CPositionFromImageSupplierComp:
			public iproc::TSupplierCompWrap<imath::CVarVector>,
			virtual public iproc::IValueProvider
{
public:
	typedef iproc::TSupplierCompWrap<imath::CVarVector> BaseClass;

	I_BEGIN_COMPONENT(CPositionFromImageSupplierComp);
		I_REGISTER_INTERFACE(iproc::IValueProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide image to analyse", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_processorCompPtr, "Processor", "Processor calculating set of positions from image", true, "Processor");
	I_END_COMPONENT;

	// reimplemented (iproc::IValueProvider)
	virtual imath::CVarVector GetValue(int index = -1, int valueTypeId = VTI_AUTO) const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(imath::CVarVector& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iipr::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
};


} // namespace iipr


#endif // !iipr_CPositionFromImageSupplierComp_included


