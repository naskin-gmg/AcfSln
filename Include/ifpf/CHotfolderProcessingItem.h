#ifndef ifpf_CHotfolderProcessingItem_included
#define ifpf_CHotfolderProcessingItem_included


// ACF includes
#include "imod/TModelWrap.h"

#include "iimg/CGeneralBitmap.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingItem.h"


namespace ifpf
{


/**
	Interface for a single processing item in the hotfolder.
*/
class CHotfolderProcessingItem: virtual public ifpf::IHotfolderProcessingItem
{
public:
	CHotfolderProcessingItem();

	// reimplemented (ifpf::IHotfolderProcessingItem)
	virtual int GetProcessingState() const;
	virtual void SetProcessingState(int processingState);
	virtual double GetProgress() const;
	virtual void SetProgress(double progress);
	virtual istd::CString GetInputFile() const;
	virtual void SetInputFile(const istd::CString& inputFile);
	virtual istd::CString GetOutputFile() const;
	virtual void SetOutputFile(const istd::CString& outputFile);
	virtual const iimg::IBitmap& GetInputPreview() const;
	virtual void SetInputPreview(const iimg::IBitmap& inputPreview);
	virtual const iimg::IBitmap& GetOutputPreview() const;
	virtual void SetOutputPreview(const iimg::IBitmap& outputPreview);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_inputFile;
	istd::CString m_outputFile;
	int m_processingState;
	double m_progress;

	imod::TModelWrap<iimg::CGeneralBitmap> m_inputPreview;
	imod::TModelWrap<iimg::CGeneralBitmap> m_outputPreview;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderProcessingItem_included


