#ifndef iipr_CProcessedAcquisitionComp_included
#define iipr_CProcessedAcquisitionComp_included


#include "iproc/TSyncProcessorCompBase.h"

#include "iproc/IBitmapAcquisition.h"


namespace iipr
{


class CProcessedAcquisitionComp: public iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition>
{
public:
	typedef iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition> BaseClass;

	I_BEGIN_COMPONENT(CProcessedAcquisitionComp);
		I_ASSIGN(m_slaveAcquisitionCompPtr, "SlaveCamera", "Camera for real image acquisition", true, "SlaveCamera");
		I_ASSIGN(m_processorCompPtr, "ImageProcessor", "Image processor", false, "ImageProcessor");
	I_END_COMPONENT;

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	I_REF(iproc::IBitmapAcquisition, m_slaveAcquisitionCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
};


} // namespace iipr


#endif // !iipr_CProcessedAcquisitionComp_included


