#include "iipr/CConvolutionKernel2d.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

// IACF includes
#include "iipr/iipr.h"


namespace iipr
{


CConvolutionKernel2d::CConvolutionKernel2d()
{
}


CConvolutionKernel2d::CConvolutionKernel2d(const imath::ISampledFunction2d& function2d)
:	BaseClass(function2d)
{
}


// reimplemented (iipr::IConvolutionKernel2d)

istd::CIndex2d CConvolutionKernel2d::GetKernelSize() const
{
	return GetGridSize2d();
}


double CConvolutionKernel2d::GetKernelElement(const istd::CIndex2d& index) const
{
	return GetSampleValue(index);
}


bool CConvolutionKernel2d::SetKernelElement(const istd::CIndex2d& index, double value)
{
	SetSampleValue(index, value);

	return true;
}


// reimplemented (iser::ISerializable)

bool CConvolutionKernel2d::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag sizeTag("KernelSize", "Size of convolution kernel");
	static iser::CArchiveTag widthTag("Width", "Width");
	static iser::CArchiveTag heightTag("Height", "Height");
	static iser::CArchiveTag valuesTag("Values", "values of convolution kernel");

	bool retVal = true;

	if (archive.IsStoring()){
		istd::CIndex2d size = GetGridSize2d();

		retVal = retVal && archive.BeginTag(sizeTag);

		retVal = retVal && archive.BeginTag(widthTag);
		retVal = retVal && archive.Process(size[0]);
		retVal = retVal && archive.EndTag(widthTag);

		retVal = retVal && archive.BeginTag(heightTag);
		retVal = retVal && archive.Process(size[1]);
		retVal = retVal && archive.EndTag(heightTag);

		retVal = retVal && archive.EndTag(sizeTag);

		retVal = retVal && archive.BeginTag(valuesTag);

		istd::CIndex2d index;
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			for (index[1] = 0; index[1] < size[0]; ++index[1]){
				double value = GetSampleValue(index);
				retVal = retVal && archive.Process(value);
			}
		}

		retVal = retVal && archive.EndTag(valuesTag);
	}
	else{
		istd::CIndex2d size(0, 0);

		retVal = retVal && archive.BeginTag(sizeTag);

		retVal = retVal && archive.BeginTag(widthTag);
		retVal = retVal && archive.Process(size[0]);
		retVal = retVal && archive.EndTag(widthTag);

		retVal = retVal && archive.BeginTag(heightTag);
		retVal = retVal && archive.Process(size[1]);
		retVal = retVal && archive.EndTag(heightTag);

		retVal = retVal && archive.EndTag(sizeTag);

		if (!CreateGrid2d(size)){
			return false;
		}

		retVal = retVal && archive.BeginTag(valuesTag);

		istd::CIndex2d index;
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			for (index[1] = 0; index[1] < size[0]; ++index[1]){
				double value = 0;
				retVal = retVal && archive.Process(value);
				if (!retVal){
					return false;
				}

				SetSampleValue(index, value);
			}
		}

		retVal = retVal && archive.EndTag(valuesTag);
	}

	return retVal;
}


I_DWORD CConvolutionKernel2d::GetMinimalVersion(int versionId) const
{
	if (versionId == IacfVersionId){
		return 405;
	}

	return 0;
}


} // namespace iipr


