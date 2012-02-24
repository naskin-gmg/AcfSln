#ifndef ifpf_CHotfolderProcessingItem_included
#define ifpf_CHotfolderProcessingItem_included


// ACF includes
#include "imod/TModelWrap.h"

#include "isys/CSimpleDateTime.h"


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
	virtual std::string GetItemUuid() const;
	virtual int GetProcessingState() const;
	virtual void SetProcessingState(int processingState);
	virtual QString GetInputFile() const;
	virtual void SetInputFile(const QString& inputFile);
	virtual QString GetOutputFile() const;
	virtual void SetOutputFile(const QString& outputFile);
	virtual double GetProcessingTime() const;
	virtual void SetProcessingTime(double processingTime);
	virtual const isys::IDateTime& GetStartTime() const;
	virtual void SetStartTime(const isys::IDateTime& startTime);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QString m_inputFile;
	QString m_outputFile;
	int m_processingState;
	double m_processingTime;
	isys::CSimpleDateTime m_startTime;

	mutable std::string m_itemId;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderProcessingItem_included


