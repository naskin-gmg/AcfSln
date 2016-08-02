#include "iipr/CConvolutionKernel2d.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

// IACF includes
#include "iipr/iipr.h"


namespace iipr
{


// static constants
static const iser::CArchiveTag sizeTag("KernelSize", "Size of convolution kernel", iser::CArchiveTag::TT_GROUP);
static const iser::CArchiveTag widthTag("Width", "Width", iser::CArchiveTag::TT_LEAF, &sizeTag);
static const iser::CArchiveTag heightTag("Height", "Height", iser::CArchiveTag::TT_LEAF, &sizeTag);
static const iser::CArchiveTag valuesTag("Values", "values of convolution kernel", iser::CArchiveTag::TT_GROUP);
static const iser::CArchiveTag offsetValueTag("Offset", "Offset value", iser::CArchiveTag::TT_LEAF);


CConvolutionKernel2d::CConvolutionKernel2d()
:	m_valueOffset(0)
{
}


CConvolutionKernel2d::CConvolutionKernel2d(const CConvolutionKernel2d& kernel)
:	BaseClass(kernel),
	m_valueOffset(kernel.m_valueOffset)
{
}


CConvolutionKernel2d::CConvolutionKernel2d(const imath::ISampledFunction2d& function2d)
:	BaseClass(function2d),
	m_valueOffset(0)
{
}


// reimplemented (iipr::IConvolutionKernel2d)

istd::CIndex2d CConvolutionKernel2d::GetKernelSize() const
{
	return GetGridSize2d();
}


void CConvolutionKernel2d::SetKernelSize(const istd::CIndex2d& kernelSize)
{
	CreateGrid2d(kernelSize, 1.0);
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


double CConvolutionKernel2d::GetOffsetValue() const
{
	return m_valueOffset;
}


void CConvolutionKernel2d::SetOffsetValue(double offset)
{
	if (offset != m_valueOffset){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_valueOffset = offset;
	}
}


// reimplemented (iser::ISerializable)

bool CConvolutionKernel2d::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	bool isStoring = archive.IsStoring();
	istd::CChangeNotifier notifier(isStoring? NULL: this);
	Q_UNUSED(notifier);

	if (isStoring){
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
			for (index[1] = 0; index[1] < size[1]; ++index[1]){
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
			for (index[1] = 0; index[1] < size[1]; ++index[1]){
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

	quint32 versionNumber = 0;
	if (!archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, versionNumber) || (versionNumber >= 1621)){
		retVal = retVal && archive.BeginTag(offsetValueTag);
		retVal = retVal && archive.Process(m_valueOffset);
		retVal = retVal && archive.EndTag(offsetValueTag);
	}
	else if (!isStoring){
		m_valueOffset = 0;
	}

	return retVal;
}


quint32 CConvolutionKernel2d::GetMinimalVersion(int versionId) const
{
	if (versionId == AcfSlnVersionId){
		if (m_valueOffset != 0){
			return 1621;
		}
	}

	return 0;
}


} // namespace iipr


