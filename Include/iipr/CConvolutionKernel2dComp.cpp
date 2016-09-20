#include <iipr/CConvolutionKernel2dComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>

#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>

// IACF includes
#include <iipr/iipr.h>


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CConvolutionKernel2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_kernelAttrPtr.IsValid() && m_kernelWidthAttrPtr.IsValid() && m_kernelHeightAttrPtr.IsValid()){
		int kernelSize = m_kernelAttrPtr.GetCount();
		int kernelWidth = *m_kernelWidthAttrPtr;
		int kernelHeight = *m_kernelHeightAttrPtr;

		if (kernelWidth * kernelHeight != kernelSize){
			qDebug("Given kernel dimensions do not match the kernel size");

			return;
		}

		BaseClass2::SetKernelSize(istd::CIndex2d(kernelWidth, kernelHeight));

		for (int y = 0; y < kernelHeight; y++){
			for (int x = 0; x < kernelWidth; x++){

				int kernelIndex = x + y * kernelWidth;

				double kernelValue = m_kernelAttrPtr[kernelIndex];

				BaseClass2::SetKernelElement(istd::CIndex2d(x, y), kernelValue);			
			}		
		}
	}
}


} // namespace iipr


