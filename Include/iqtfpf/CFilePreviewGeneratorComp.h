#ifndef iqtfpf_CFilePreviewGeneratorComp_included
#define iqtfpf_CFilePreviewGeneratorComp_included


// ACF includes
#include "iser/IFileLoader.h"

#include "iproc/IBitmapAcquisition.h"
#include "iproc/TSyncProcessorCompBase.h"


// AcfSln includes
#include "ibase/IObjectSnap.h"


namespace iqtfpf
{


class CFilePreviewGeneratorComp: public iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition>
{
public:
	typedef iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition> BaseClass;

	I_BEGIN_COMPONENT(CFilePreviewGeneratorComp);
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader, which will used to open the input file", true, "FileLoader");
		I_ASSIGN(m_fileDataCompPtr, "FileData", "Data component for the loader", true, "FileData");
		I_ASSIGN(m_objectSnapCompPtr, "ObjectSnap", "Component for bitmap snap of a concrete object", true, "ObjectSnap");
		I_ASSIGN(m_widthAttrPtr, "BitmapWidth", "Width of the generated preview bitmap", true, 128);
		I_ASSIGN(m_heightAttrPtr, "BitmapHeight", "Height of the generated preview bitmap", true, 128);
	I_END_COMPONENT();

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr);
private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_REF(istd::IChangeable, m_fileDataCompPtr);
	I_REF(ibase::IObjectSnap, m_objectSnapCompPtr);
	I_ATTR(int, m_widthAttrPtr);
	I_ATTR(int, m_heightAttrPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CFilePreviewGeneratorComp_included


