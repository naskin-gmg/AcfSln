#ifndef ifpf_CHotfolderProcessingItem_included
#define ifpf_CHotfolderProcessingItem_included


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

	// reimplemented ()ifpf::CHotfolderProcessingItem
	virtual int GetProcessingState() const;
	virtual void SetProcessingState(int processingState);
	virtual double GetProgress() const;
	virtual void SetProgress(double progress);
	virtual istd::CString GetInputFile() const;
	virtual void SetInputFile(const istd::CString& inputFile);
	virtual istd::CString GetOutputFile() const;
	virtual void SetOutputFile(const istd::CString& outputFile);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_inputFile;
	istd::CString m_outputFile;
	int m_processingState;
	double m_progress;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderProcessingItem_included


